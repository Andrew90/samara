#include "stdafx.h"
#include "VersionDiff.h"
#include "AppBase.h"
#include "USPCData.h"


namespace Version
{
	static const int __magic_word__ = 0xcafe0000;
	static const int __version__ = 3;

	struct ScopeVelocity
	{
		double value[App::count_sensors];
	};
	ScopeVelocity stored_items;

	typedef TL::MkTlst<ThresholdsTable, DeadAreaTable, MedianFiltreTable, AxesTable>::Result items_list;
	TL::Factory<items_list> stored_tables;

	template<class T>struct __data__
	{
		FILE *f;
		typename T::TItems &items;
		__data__(FILE *f)
			: f(f)
			, items(Singleton<T>::Instance().items)
		{}
	};

	template<class O, class P>struct __read__
	{
		void operator()(P &p)
		{
			fread(&p.items.get<O>().value, sizeof(O::type_value), 1, p.f);
		}
	};
// —читывание 
	template<class O, class P>struct __read_noop__
	{
		void operator()(P &p)
		{
			O::type_value noop;
			fread(noop, sizeof(O::type_value), 1, p.f);
		}
	};
// —охранение в файле порогов
	template<class O, class P>struct __write__
	{
		void operator()(P &p)
		{
			fwrite(&p.items.get<O>().value, sizeof(O::type_value), 1, p.f);
		}
	};

	template<class O, class P>struct __read_all__
	{
		void operator()(P *p)
		{
			TL::foreach<typename O::items_list, __read__>()(__data__<O>(p));
		}
	};
	template<class O, class P>struct __write_all__
	{
		void operator()(P *p)
		{
			TL::foreach<typename O::items_list, __write__>()(__data__<O>(p));
		}
	};

	void SaveToFile(FILE *f)
	{
		unsigned t = __magic_word__ | __version__;
		fwrite(&t, sizeof(unsigned), 1, f);
		ItemData<Thickness> &d = Singleton<ItemData<Thickness>>::Instance();
		fwrite(d.scope_velocity, sizeof(d.scope_velocity), 1, f);

		wchar_t *s = Singleton<ParametersTable>::Instance().items.get<NameParam>().value;
		fwrite(s, sizeof(NameParam::type_value), 1, f);

		TL::foreach<items_list, __write_all__>()(f);
	}

	bool LoadFromFile(unsigned ver, FILE *f)
	{
		if(__magic_word__ == (ver & 0xffff0000))
		{
			switch(ver & 0xffff)
			{
			case 3:
				{
					ItemData<Thickness> &d = Singleton<ItemData<Thickness>>::Instance();
					fread(d.scope_velocity, sizeof(d.scope_velocity), 1, f);

					wchar_t *s = Singleton<ParametersTable>::Instance().items.get<NameParam>().value;
					fread(s, sizeof(NameParam::type_value), 1, f);

					TL::foreach<items_list, __read_all__>()(f);
				}
				return true;
			case 2:
				{
					ItemData<Thickness> &d = Singleton<ItemData<Thickness>>::Instance();
					fread(d.scope_velocity, sizeof(d.scope_velocity), 1, f);

					wchar_t *s = Singleton<ParametersTable>::Instance().items.get<NameParam>().value;
					fread(s, sizeof(NameParam::type_value), 1, f);
				}
				return true;
			case 1:
				{
					ItemData<Thickness> &d = Singleton<ItemData<Thickness>>::Instance();
					fread(d.scope_velocity, sizeof(d.scope_velocity), 1, f);
					Singleton<ParametersTable>::Instance().items.get<NameParam>().value = L"default";
				}
				return true;
			}
		}
		Singleton<ParametersTable>::Instance().items.get<NameParam>().value = L"default";
		return false;
	}

	void RestoreMemory()
	{
		ItemData<Thickness> &d = Singleton<ItemData<Thickness>>::Instance();
		memcpy(d.scope_velocity, stored_items.value, sizeof(d.scope_velocity)); 
	}
	//void StoreMemory(){}
	void StoreScopeVelocity(int i, double data)
	{
		stored_items.value[i] = data;
	}
}
