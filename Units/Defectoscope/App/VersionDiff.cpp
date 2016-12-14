#include "stdafx.h"
#include "VersionDiff.h"
#include "AppBase.h"
#include "USPCData.h"
#include "DebugMess.h"


namespace Version
{
	static const int __magic_word__ = 0xcafe0000;
	static const int __version__ = 5;

	struct ScopeVelocity
	{
		double value[App::count_sensors];
	};
	ScopeVelocity stored_items;

	void SaveToFile(FILE *f)
	{
		unsigned t = __magic_word__ | __version__;
		fwrite(&t, sizeof(unsigned), 1, f);
		{
			ItemData<Cross> &d = Singleton<ItemData<Cross>>::Instance();
			fwrite(d.param, sizeof(d.param), 1, f);
		}
		{
			ItemData<Long> &d = Singleton<ItemData<Long>>::Instance();
			fwrite(d.param, sizeof(d.param), 1, f);
		}
		{
			ItemData<Thickness> &d = Singleton<ItemData<Thickness>>::Instance();
			fwrite(d.param, sizeof(d.param), 1, f);
		}
	}

	class USPCIniFile
	{
	public:
	   double scope_range    [8];
	   double scope_offset   [8];
	   double gateIF_position[8];
	   double gateIF_width   [8];
	   double gateIF_level   [8];
	   double gate1_width    [8];
	   double gate1_position [8];
	   double gate1_level    [8];
	};

	bool LoadFromFile(unsigned ver, FILE *f)
	{
		if(__magic_word__ == (ver & 0xffff0000))
		{
			int v = ver & 0xffff;
			switch(v)
			{
			case __version__:
				{
					{
						ItemData<Cross> &d = Singleton<ItemData<Cross>>::Instance();
						fread(d.param, sizeof(d.param), 1, f);
					}
					{
						ItemData<Long> &d = Singleton<ItemData<Long>>::Instance();
						fread(d.param, sizeof(d.param), 1, f);
					}
					{
						ItemData<Thickness> &d = Singleton<ItemData<Thickness>>::Instance();
						fread(d.param, sizeof(d.param), 1, f);
					}
				}
				break;
			case 4:
				{
					double scope_velocity[App::count_sensors];
					fread(scope_velocity, sizeof(scope_velocity), 1, f);
					
					USPCIniFile u;// = Singleton<USPCIniFile>::Instance();
	            	fread(&u, sizeof(USPCIniFile), 1, f);
					dprint("ok");
				}
				return true;
			case 2:
				{
					double scope_velocity[App::count_sensors];
					fread(scope_velocity, sizeof(scope_velocity), 1, f);

					//wchar_t *s = Singleton<ParametersTable>::Instance().items.get<NameParam>().value;
					NameParam::type_value s;
					fread(s, sizeof(NameParam::type_value), 1, f);
				}
				return true;
			case 1:
				{
					//ItemData<Thickness> &d = Singleton<ItemData<Thickness>>::Instance();
					double x[8];
					fread(x, sizeof(x), 1, f);
				}
				return true;
			}
		}
		return false;
	}

	void RestoreMemory()
	{
		//ItemData<Thickness> &d = Singleton<ItemData<Thickness>>::Instance();
		//memcpy(d.scope_velocity, stored_items.value, sizeof(d.scope_velocity)); 
	}
	//void StoreMemory(){}
	void StoreScopeVelocity(int i, double data)
	{
		//stored_items.value[i] = data;
	}
}
