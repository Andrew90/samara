#pragma once

#include "message.h"
#include "DebugMess.h"
#include "ParamDlg.h"

template<class Table, class List>class TabPage
{
	List &owner;
	HWND hTab;
	bool create;
	template<class O, class P>struct __show__
	{
		void operator()(O *o, P *p)
		{
			ShowItem()(o->hWnd, *p);
		}
	};
	template<class O, class P>struct __init__
	{
		void operator()(O *o, P *p)
		{
			o->Init(p->h, p->width, p->height);	
		}
	};
	struct __table_data__
	{
		HWND h; 
		int &width; 
		int &height;
		__table_data__(HWND h, int &width, int &height): h(h), width(width), height(height){}
	};
	typedef Table TableParam;
	Table *table;
public:
	typedef typename TL::TypeToTypeLst<typename Table::items_list, DlgItem>::Result list;
	TL::Factory<list> items;
	void Show(bool b)
	{
		if(create)
		{
			TL::foreach<list, __show__>()(&items, &b);
		}
		else
		{
			create = true;
			int width = 450;
			int height = 30;
			TL::foreach<list, __init__>()(&items, &__table_data__(owner.hTab, width, height));
		}
	}
	
public:
	TabPage(List &l) 
		: owner(l)
		, create(false)
		, table((Table *)(l.data.data[TL::IndexOf<typename List::Table::items_list, Table>::value]))
		, items(table->items)
	{}	
};

//--------------------------------------------------------------------------------------------------------
#pragma warning(disable: 4355)
template<class T, class ButtonsList = TL::MkTlst<OkBtn, CancelBtn>::Result>class TabControl
{
	struct __command_data__
	{
		TMessage &mes;
		TabControl &owner;
		__command_data__(TMessage &mes, TabControl &owner)
			: mes(mes)
			, owner(owner)
		{}
	};
	template<class O, class P>struct __command__
	{
		bool operator()(O *o, P *p)
		{
			if(p->mes.wParam == O::ID)
			{
				o->BtnHandler(p->owner, p->mes.hwnd);
				return false;
			}
			return true;
		}
	};
	template<class List>struct __btn_width__;
	template<class Head, class Tail>struct __btn_width__<Tlst<Head, Tail> >
	{
		static const int value = Head::width + __btn_width__<Tail>::value;
	};
	template<>struct __btn_width__<NullType>
	{
		static const int value = 0;
	};
	struct __make_btn_data__
	{
		int offs;
		int height;
		HWND h;
		__make_btn_data__(int offs, int height, HWND h)
			: offs(offs)
			, height(height)
			, h(h)
		{}
	};
	template<class O, class P>struct __tab_name__
	{
		void operator()(O *, P *hTab)
		{
			TCITEM ti = {0};
			ti.mask = TCIF_TEXT;
			ti.pszText = ParamTitle<O>()();
			ti.cchTextMax = wcslen(ti.pszText);
			int iid = TL::IndexOf<typename T::items_list, O>::value;
			int res = SendMessage(*hTab, TCM_INSERTITEM, iid, (LPARAM)&ti);
		}
	};
	struct __select_page_data__
	{
		int id;
		bool show;
		TabControl *self;
	};
	template<class O, class P>struct __items__
	{
		void operator()(O *o, P *p)
		{
			Show(o->hWnd, p->show);
		}
	};
	template<class O, class P>struct __select_page__
	{
		bool operator()(O *, P *p)
		{
			if(p->id == TL::IndexOf<typename T::items_list, O>::value)
			{
				p->self->items.get<TabPage<O, TabControl> >().Show(p->show);
				return false;
			}
			return true;
		}
	};
	TL::Factory<ButtonsList> buttons;
	void Show(bool b)
	{
		int iPage = TabCtrl_GetCurSel(hTab);
		__select_page_data__ data = {iPage, b, this};
		TL::find<typename Table::items_list, __select_page__>()((TL::Factory<typename Table::items_list> *)0, &data);
	}
public:
	HWND hWnd;
	HWND hTab;
	typedef T Table;
	T &data;
	int x, y, width, height;
	typedef typename TL::TypeToTypeLstParam1<typename Table::items_list, TabPage, TabControl>::Result list;
	TL::Factory<list> items;
	TabControl(T &t, int w, int h)
		: data(t)
		, items(*this)
		, width(w)
		, height(h)
	{} 
	void Init(HWND h)
	{
		hWnd = h;

		int offs = __btn_width__<ButtonsList>::value + (TL::Length<ButtonsList>::value - 1) * 10;

		offs = (width - offs) / TL::Length<ButtonsList>::value;
		RECT r;
		GetWindowRect(GetDesktopWindow(), &r);

		x = r.left +(r.right - r.left - width) / 2;
		y = r.top +(r.bottom - r.top - height) / 2;
		MoveWindow(h, x, y, width, height, FALSE);
		hTab = CreateWindowEx(0, WC_TABCONTROL, 0,   WS_CHILD | WS_VISIBLE, 
			0, 0, width, height - 70, h, (HMENU)NULL, hInstance, 0);

		//unsigned flags = SWP_NOSIZE | SWP_DRAWFRAME;

		//SetWindowPos(hTab, WS_EX_TOPMOST, 0, 0, width, height - 70, flags);

		TL::foreach<typename T::items_list, __tab_name__>()((TL::Factory<typename T::items_list> *)0, (HWND *)&hTab);

		TL::foreach<ButtonsList, __make_btn__>()(&buttons, &__make_btn_data__(offs, height - 65, h));
		((TabControl *)GetWindowLong(h, GWL_USERDATA))->Show(true);
	}
	void Command(TMessage &h)
	{
		TL::find<ButtonsList, __command__>()(&buttons, &__command_data__(h, *this));
	}
	bool Notify(TNotify &l)
	{
		switch (l.pnmh->code)
		{
		case TCN_SELCHANGING:
			{
				((TabControl *)GetWindowLong(l.hwnd, GWL_USERDATA))->Show(false);
			}
			return FALSE;

		case TCN_SELCHANGE:
			{ 
				((TabControl *)GetWindowLong(l.hwnd, GWL_USERDATA))->Show(true);
			} 
			break;
		}
		return TRUE;
	}
	  static LRESULT CALLBACK Proc(HWND h, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch(msg)
		{
		case WM_COMMAND:
			{
				((TabControl *)GetWindowLong(h, GWL_USERDATA))->Command((TMessage &)h);
			}
			return TRUE;
		case WM_INITDIALOG:
			{	
				SetWindowLong(h, GWL_USERDATA, lParam);
				((TabControl *)lParam)->Init(h);				
			}
			return TRUE;

		case WM_NOTIFY:	 return ((TabControl *)lParam)->Notify((TNotify &)h);
		}
		return FALSE;
	}
	  bool Do(HWND hWnd, wchar_t *title)
	{
		return TemplDlg_Do(hWnd, title, (DLGPROC)Proc, (LPARAM)this);
	}
};