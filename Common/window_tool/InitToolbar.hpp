#pragma once
#include <windows.h>
#include <commctrl.h>
#include "typelist.hpp"
#include "Resource.h"
extern HINSTANCE hInstance;

template<class T>struct __tool_btn_width__
{
	static const int value = 10;
};
template<int idb, void(*ptr)(HWND), class z, template<int, void(*)(HWND), class>class W>struct __tool_btn_width__<W<idb, ptr, z> > 
{
	static const int value = 71;
};
template<class List>struct __tool_bar_width__;
template<class Head, class Tail>struct __tool_bar_width__<Tlst<Head, Tail> >
{
	static const int value = __tool_btn_width__<Head>::value + __tool_bar_width__<Tail>::value;
};
template<>struct __tool_bar_width__<NullType>
{
	static const int value = 0;
};	

//struct ToolTipDefault{static wchar_t *Do(){return L"";}};
template<int idb, void(*ptr)(HWND), class TOOLTIP = NullType>struct ButtonToolbar
{
	static const int IDB_ = idb;
	static const int TBSTYLE_ = TBSTYLE_BUTTON | BTNS_SHOWTEXT;
	typedef TOOLTIP Text;
	static void Do(HWND h)
	{
		(*ptr)(h);
	}
};
template<int N>struct SeparatorToolbar
{
   static const int IDB_ = IDB_SEP;
   static const int TBSTYLE_ = TBSTYLE_SEP;
};

template<class T>class Text
{
	template<bool, class T>struct sub
	{
		static wchar_t *Do(){return T::Text();};
	};
	template<class T>struct sub<false, T>
	{
		static wchar_t *Do(){return 0;};
	};
	template<class Z, Z>struct Helper{};
	template<class Z>static char Is(Z *,  Helper<wchar_t *(*)(), &Z::Text>* = NULL);
	template<class Z>static double Is(...);
	static const bool value =  sizeof(char) == sizeof(Is<T>((T *)0));
public:
	wchar_t *operator()(){return sub<sizeof(char) == sizeof(Is<T>((T *)0)), T>::Do();}
};

template<class list, int Height = 64, int SEPARATOR_WIDTH = 10>class InitToolbar
{
	typedef InitToolbar Self;
	HIMAGELIST himl;
	TBBUTTON tbb[TL::Length<list>::value];
    template<class T>struct no_IDB_SEP
	{
		static const bool value = IDB_SEP != T::IDB_;
	};
public:
	typedef typename TL::EraseAllParam<list, no_IDB_SEP>::Result only_buttons_list;	
private:
	template<int>struct Set
	{
		template<class O>void Do(TBBUTTON &tbb, Self *self)
		{
			HBITMAP hbmp = LoadBitmap(hInstance, MAKEINTRESOURCE(O::IDB_)); 
			ImageList_AddMasked(self->himl,hbmp, RGB(255,255,255));  
			DeleteObject(hbmp);

			tbb.iBitmap  = TL::IndexOf<only_buttons_list, O>::value;
			tbb.dwData = (DWORD_PTR)O::Do;
			tbb.idCommand =  O::IDB_;				
			tbb.fsState = TBSTATE_ENABLED;
			tbb.fsStyle = O::TBSTYLE_;
			//static const wchar_t *s = O::ToolLip::Do();
			//tbb.iString = (INT_PTR)(0 == s[0] ? 0 : s);
			tbb.iString = (INT_PTR)Text<O::Text>()();
		}
	};
	template<>struct Set<IDB_SEP>
	{
		template<class O>void Do(TBBUTTON &tbb, Self *self)
		{
			tbb.iBitmap  = SEPARATOR_WIDTH;
			tbb.fsStyle = O::TBSTYLE_; 
		}
	};
	template<class O, class P>struct loc
	{
		void operator()(O *, P *p)
		{
			Set<O::IDB_>().Do<O>(p->tbb[TL::IndexOf<list, O>::value], p);
		};
	};	    
public:
	HWND operator()(HWND hwnd)
	{
		memset(tbb, 0, sizeof(tbb));
		himl = ImageList_Create(Height, Height, ILC_COLOR24|ILC_COLORDDB|ILC_MASK, TL::Length<only_buttons_list>::value, 0);
		TL::foreach<list, loc>()((TL::Factory<list> *)0, this);
		HWND hToolBar = CreateToolbarEx(hwnd, TBSTYLE_FLAT | CCS_ADJUSTABLE | CCS_NODIVIDER | WS_CHILD | WS_VISIBLE
			, (UINT)tbb
			, TL::Length<list>::value, hInstance, NULL
			, tbb, TL::Length<list>::value,0,0,0,0, sizeof(TBBUTTON)
			);
		//Свяжите image list с ToolBar -ом
		SendMessage(hToolBar, TB_SETIMAGELIST, 0, (LPARAM)himl);
		SendMessage(hToolBar, TB_AUTOSIZE, 0, 0);
		return hToolBar;
	}	
};
/*
struct ToolTipDefault{static wchar_t *Do(){return L"";}};
template<int idb, void(*ptr)(HWND), class TOOLTIP = ToolTipDefault>struct ButtonToolbar
{
	static const int IDB_ = idb;
	static const int TBSTYLE_ = TBSTYLE_BUTTON | BTNS_SHOWTEXT;
	typedef TOOLTIP ToolLip;
	static void Do(HWND h)
	{
		(*ptr)(h);
	}
};
*/
template<class T>struct __button_tool_bar__
{
	static const int IDB_ = T::IDB_;
	static const int TBSTYLE_ = TBSTYLE_BUTTON | BTNS_SHOWTEXT;
	typedef T Text;
	static void Do(HWND h)
	{
		T::Proc(h);
	}
};

template<class List>struct MkToolBarLst;

template<class Head, class Tail>struct MkToolBarLst<Tlst<Head, Tail> >
{
	typedef Tlst<
		typename TL::_if<IDB_SEP != Head::IDB_, __button_tool_bar__<Head>, Head>::Result
		, typename MkToolBarLst<Tail>::Result
	> Result;
};

template<>struct MkToolBarLst<NullType>
{
   typedef NullType Result;
};