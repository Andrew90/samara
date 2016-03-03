#pragma once
#include "DebugMess.h"

namespace Common
{
	template<class O, class P>struct __create_window__
	{
		void operator()(O *o, P *p)
		{
			wchar_t name[256];
			const char *s = &(typeid(O).name())[6];
			int len = 1 + strlen(s);
			size_t converted;
			mbstowcs_s(&converted, name, s, len);
			o->hWnd = CreateChildWindow(*p, (WNDPROC)&Viewer<O>::Proc, name, o);
		}
	};

	template<class O, class P>struct __in_rect__
	{
		bool operator()(O *o, P *p)
		{
			RECT r;
			GetWindowRect(o->hWnd, &r);
			if(InRect(p->l.x, p->l.y, r))
			{
				p->l.hwnd = o->hWnd;
				SendMessage(MESSAGE(p->l));
				return false;
			}
			return true;
		}
	};
	template<class T, class Owner>struct __event_data__
	{
		Owner &owner;
		T &l;
		__event_data__(Owner &owner, T &l)
			:	owner(owner)
			, l(l)
		{}
	};

	template<class O, class P>struct __sub_in_rect__
	{
		void operator()(O *o, P *p)
		{
			RECT r;
			GetWindowRect(o->hWnd, &r);
			p->l.hwnd = o->hWnd;
			o->storedMouseMove.x = p->l.x;
			SendMessage(MESSAGE(p->l));
		}
	};

	template<class O, class P, class List>struct __in_rect_all__
	{
		bool operator()(O *o, P *p)
		{
			RECT r;
			GetWindowRect(o->hWnd, &r);
			if(InRect(p->l.x, p->l.y, r))
			{
				p->l.hwnd = o->hWnd;
				SendMessage(MESSAGE(p->l));

				p->l.x = o->storedMouseMove.x;
				p->l.delta = 0;
				typedef TL::EraseItem<List, O>::Result lst;
				TL::foreach<lst, __sub_in_rect__>()(&p->owner.viewers, p);
				return false;
			}
			return true;
		}
	};

	template<class O, class P>struct __sub_in_rect_wapper_
	{
		void operator()(O *o, P *p)
		{
			RECT r;
			GetWindowRect(o->hWnd, &r);
			o->offsetX = p->offsetX;
			TMouseWell m = {o->hWnd, WM_MOUSEWHEEL, 0, 0, 0};
			SendMessage(MESSAGE(m));
		}
	};

	template<template<class, int>class W, class X, int N, class P, class List>struct __in_rect_all__<W<X,N>, P, List>
	{
		typedef W<X,N> O;
		bool operator()(O *o, P *p)
		{
			RECT r;
			GetWindowRect(o->hWnd, &r);
			if(InRect(p->l.x, p->l.y, r))
			{
				p->l.hwnd = o->hWnd;
				SendMessage(MESSAGE(p->l));

				p->l.x = o->storedMouseMove.x;
				p->l.delta = 0;
				typedef TL::EraseItem<List, O>::Result lst;
				TL::foreach<lst, __sub_in_rect_wapper_>()(&p->owner.viewers, o);
				return false;
			}
			return true;
		}
	};

	template<class T>bool DestroyWindow(HWND h)
	{
		HWND hh = FindWindow(WindowClass<T>()(), 0);
		if(NULL != hh)
		{
			DestroyWindow(hh);
			return true;
		}
		return false;
	}

	template<class T>struct OpenWindow
	{
		static void Do(HWND)
		{
			HWND hh = FindWindow(WindowClass<T>()(), 0);
			if(NULL != hh)
			{
				SendMessage(hh, WM_SYSCOMMAND, SC_RESTORE, 0);
				SetForegroundWindow(hh);
			}
			else
			{
				RECT r;
				WindowPosition::Get<T>(r);
				HWND h = WindowTemplate(&Singleton<T>::Instance(), T::Title(), r.left, r.top, r.right, r.bottom);
				ShowWindow(h, SW_SHOWNORMAL);
			}
		}
	};	
}

