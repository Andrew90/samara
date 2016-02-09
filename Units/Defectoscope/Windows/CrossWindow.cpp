#include "stdafx.h"
#include "CrossWindow.h"
#include "Emptywindow.h"

CrossWindow::CrossWindow()
{
}
namespace
{
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
			 MoveWindow(o->hWnd, 0, 0, p->width, 100, FALSE);
		}
	};
	template<int N, class P>struct __draw__<CrossWindow::Line<N>, P>
	{
		typedef CrossViewer O;
		void operator()(O *o, P *p)
		{
			bool b = N % 2 == 0;
			int x = b 
				? 0
				: p->width / 2
				;
			 MoveWindow(o->hWnd, x, p->y, p->width / 2, p->height, FALSE);
			 if(!b) p->y += p->height;
		}
	};
}
void CrossWindow::operator()(TSize &l)
{
    RECT r;
	GetClientRect(l.hwnd, &r);

	__data__ data = {0, r.right,  (r.bottom - 100) / 4};
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
unsigned CrossWindow::operator()(TCreate &)
{
	return 0;
}
void CrossWindow::operator()(TMouseWell &)
{
}
void CrossWindow::Do(HWND)
{
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