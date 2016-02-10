#include "stdafx.h"
#include "CrossWindow.h"
#include "Emptywindow.h"
#include "WindowsPosition.h"
#include "CommonWindows.h"
#include "DebugMess.h"

CrossWindow::CrossWindow()
{
}
namespace
{
	static const int cross_window_height = 200;
	struct __data__
	{
		int y, width, height;
	};
	template<class O, class P>struct __draw__;
	template<class P>struct __draw__<CrossViewer, P>
	{
		typedef CrossViewer O;
		void operator()(O *o, P *p)
		{
			 MoveWindow(o->hWnd, 0, 0, p->width, cross_window_height, TRUE);
		}
	};
	template<int N, class P>struct __draw__<CrossWindow::Line<N>, P>
	{
		typedef CrossWindow::Line<N> O;
		void operator()(O *o, P *p)
		{
			bool b = N % 2 == 0;
			int x = b 
				? 0
				: p->width / 2
				;
			 MoveWindow(o->hWnd, x, p->y, p->width / 2, p->height, TRUE);
			 if(!b) p->y += p->height;
		}
	};
}
void CrossWindow::operator()(TSize &l)
{
    RECT r;
	GetClientRect(l.hwnd, &r);

	__data__ data = {cross_window_height, r.right,  (r.bottom - cross_window_height) / 4};
	TL::foreach<viewers_list, __draw__>()(&viewers, &data);
}
void CrossWindow::operator()(TCommand &)
{
}
void CrossWindow::operator()(TGetMinMaxInfo &m)
{
	if(NULL != m.pMinMaxInfo)
	{
		m.pMinMaxInfo->ptMinTrackSize.x = 600;
		m.pMinMaxInfo->ptMinTrackSize.y = 600;
	}	
}

unsigned CrossWindow::operator()(TCreate &l)
{
	TL::foreach<viewers_list, CommonWindows::__create_window__>()(&viewers, &l.hwnd);
	return 0;
}
//--------------------------------------------------------------------------------
namespace CommonWindows
{
	template<int N, class P>struct __in_rect__<CrossWindow::Line<N>, P>
	  : __in_rect_all__<CrossWindow::Line<N>, P, TL::EraseItem<CrossWindow::viewers_list, CrossViewer>::Result>{};
}
//----------------------------------------------------------------------------------------------
void CrossWindow::operator()(TMouseWell &l)
{
	TL::find<viewers_list, CommonWindows::__in_rect__>()(
		&viewers
		, &CommonWindows::__event_data__<TMouseWell, CrossWindow>(*this, l)
		);
}
//-----------------------------------------------------------------------------------
void CrossWindow::Do(HWND)
{
	HWND hh = FindWindow(WindowClass<CrossWindow>()(), 0);
	if(NULL != hh)
	{
		SendMessage(hh, WM_SYSCOMMAND, SC_RESTORE, 0);
		SetForegroundWindow(hh);
	}
	else
	{
		RECT r;
		WindowPosition::Get<CrossWindow>(r);
		HWND h = WindowTemplate(&Instance(), L"�������� ���������� ��������", r.left, r.top, r.right, r.bottom);
		ShowWindow(h, SW_SHOWNORMAL);
	}
}
bool CrossWindow::Destroy()
{
	HWND hh = FindWindow(WindowClass<CrossWindow>()(), 0);
	if(NULL != hh)
	{
		DestroyWindow(hh);
		return true;
	}
	return false;
}
CrossWindow &CrossWindow::Instance()
{
	static CrossWindow x; return x;
}