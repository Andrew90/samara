#include "stdafx.h"
#include "ParamDlg.h"
#include "ParamDlg.hpp"
#include "Dialogs.h"
#include "AppBase.h"
#include "App.h"
#include "Device1730.h"

template<class O, class P>struct __set__
{
	typedef typename TL::Inner<O>::Result T;
	void operator()(O *o, P *p)
	{
		wchar_t buf[128];
		GetWindowText(o->hWnd, buf, dimention_of(buf));
		o->value.value = p->Set<O>(Wchar_to<typename T::type_value>()(buf));
	}
};
template<class List>struct __data__
{
	typedef List list;
	unsigned buf[TL::Length<List>::value];
	template<class T>unsigned Set(unsigned val)
	{
		buf[TL::IndexOf<list, T>::value] = val;
		return 1 << val;
	}
};
template<class O, class P>struct __compress_bits__
{
	void operator()(O *o, P *p)
	{
		unsigned t = p->get<O>().value;
		unsigned i = 0;
		while(t >>= 1) ++i;
		o->value = i;
	}
};
struct putsDlg_OkBtn
{
	static const int width = 120;
	static const int height = 30;
	static const int ID = IDOK;
	wchar_t *Title(){return L"Применить";}
	template<class Owner>void BtnHandler(Owner &owner, HWND h)
	{
		if(TestPassword<Owner::Table>()(h))
		{
			if(!TL::find<Owner::list, __test__>()(&owner.items, &h))return;
			__data__<Owner::list> d;
			TL::foreach<Owner::list, __set__>()(&owner.items, &d);
			for(int i = 0; i < dimention_of(d.buf) - 1; ++i)
			{
				unsigned value = d.buf[i];
				for(int j = i + 1; j < dimention_of(d.buf); ++j)
				{
					if(value == d.buf[j])
					{
						MessageBox(h, L"Несколько данных ссылаются на один бит", L"Ошибка!!!", MB_ICONERROR);
						return;
					}
				}
			}
			CBase base(ParametersBase().name());
			if(base.IsOpen())
			{
				CopyFactory::Do(Singleton<Owner::Table>::Instance().items, owner.table.items);
				UpdateWhere<Owner::Table>(owner.table, base).ID(1).Execute();
			}
			EndDialog(h, TRUE);
		}
	}
};


struct DefaultBtn
{
	static const int width = 160;
	static const int height = 30;
	static const int ID = IDRETRY;
	wchar_t *Title(){return L"Встроенные настройки";}
	template<class Owner>void BtnHandler(Owner &o, HWND h)
	{
		if(TestPassword<Owner::Table>()(h))
		{
			CBase base(ParametersBase().name());
			if(base.IsOpen())
			{
				Owner::Table t;
				CopyFactory::Do(Singleton<Owner::Table>::Instance().items, t.items);
				UpdateWhere<Owner::Table>(t, base).ID(1).Execute();
			}
			EndDialog(h, TRUE);
		}
	}
};
//----------------------------------------------------------------------------
DO_NOT_CHECK(SQ0    )
DO_NOT_CHECK(SQ1    )
DO_NOT_CHECK(SQ2    )
DO_NOT_CHECK(SQ3    )
DO_NOT_CHECK(SQ4    )

PARAM_TITLE(SQ0, L"Труба на датчике SQ1(вход)")
PARAM_TITLE(SQ1, L"Труба на датчике SQ2(выход)")
PARAM_TITLE(SQ2, L"Установка включена")
PARAM_TITLE(SQ3, L"SQ3");
PARAM_TITLE(SQ4, L"SQ4");

template<>struct DlgSubItems<SQ0, unsigned >: UpDownSubItem<SQ0>{};
template<>struct DlgSubItems<SQ1, unsigned >: UpDownSubItem<SQ1>{};
template<>struct DlgSubItems<SQ2, unsigned >: UpDownSubItem<SQ2>{};
template<>struct DlgSubItems<SQ3, unsigned >: UpDownSubItem<SQ3>{};
template<>struct DlgSubItems<SQ4, unsigned >: UpDownSubItem<SQ4>{};
//----------------------------------------------------------------------------------
void InputsDlg::Do(HWND h)
{
	InputBitTable t;
	TL::foreach<InputBitTable::items_list, __compress_bits__>()(&t.items, &Singleton<InputBitTable>::Instance().items);
	if(TemplDialog<InputBitTable, TL::MkTlst<putsDlg_OkBtn, CancelBtn>::Result>(t).Do(h, L"Смещения бит входного порта"))
	{
	}
}
//-----------------------------------------------------------------------------
DO_NOT_CHECK(Y0    )
DO_NOT_CHECK(Y1    )
DO_NOT_CHECK(Y2    )
DO_NOT_CHECK(Y3    )
DO_NOT_CHECK(Y4    )

PARAM_TITLE(Y0, L"Труба на датчике Y0(вход)")
PARAM_TITLE(Y1, L"Труба на датчике Y1(выход)")
PARAM_TITLE(Y2, L"Установка включена")
PARAM_TITLE(Y3, L"Y3");
PARAM_TITLE(Y4, L"Y4");

template<>struct DlgSubItems<Y0, unsigned >: UpDownSubItem<Y0>{};
template<>struct DlgSubItems<Y1, unsigned >: UpDownSubItem<Y1>{};
template<>struct DlgSubItems<Y2, unsigned >: UpDownSubItem<Y2>{};
template<>struct DlgSubItems<Y3, unsigned >: UpDownSubItem<Y3>{};
template<>struct DlgSubItems<Y4, unsigned >: UpDownSubItem<Y4>{};

void OutputsDlg::Do(HWND h)
{
	OutputBitTable t;
	TL::foreach<OutputBitTable::items_list, __compress_bits__>()(&t.items, &Singleton<OutputBitTable>::Instance().items);
	if(TemplDialog<OutputBitTable, TL::MkTlst<putsDlg_OkBtn, CancelBtn>::Result>(t).Do(h, L"Смещения бит выходного порта"))
	{
	}
}
//----------------------------------------------------------------------------
DO_NOT_CHECK(Descriptor1730)
PARAM_TITLE(Descriptor1730, L"Дескриптор платы 1730")
void Descriptor1730Dlg::Do(HWND h)
{
	if(TemplDialog<Descriptor1730Table>(Singleton<Descriptor1730Table>::Instance()).Do(h, L"Дескриптор платы 1730"))
	{
		device1730.Destroy();
		Sleep(500);
		if(!device1730.Init(Singleton<Descriptor1730Table>::Instance().items.get<Descriptor1730>().value))
		{
			MessageBox(0, L"Не могу инициировать плату 1730", L"Ошибка !!!", MB_ICONERROR);
		}
	}
}