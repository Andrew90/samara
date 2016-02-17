#include "stdafx.h"
#include "CrossWindow.h"
#include "MenuApi.h"
#include "ViewersMenu.hpp"
#include "Emptywindow.h"
#include "Common.h"
#include "DebugMess.h"
#include "App.h"

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
	template<class O, class P>struct __set_line__
	{
		void operator()(O *, P *){}
	};
	template<int N, class P>struct __set_line__<CrossWindow::Line<N>, P>
	{
		typedef CrossWindow::Line<N> O;
		void operator()(O *o, P *p)
		{
			o->chart.items.get<BarSeries>().SetColorBarHandler(p, &CrossWindow::Draw);
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
void CrossWindow::operator()(TGetMinMaxInfo &m)
{
	if(NULL != m.pMinMaxInfo)
	{
		m.pMinMaxInfo->ptMinTrackSize.x = 600;
		m.pMinMaxInfo->ptMinTrackSize.y = 600;
	}	
}
VIEWERS_MENU(CrossWindow)
unsigned CrossWindow::operator()(TCreate &l)
{
	Menu<ViewersMenuCrossWindow::MainMenu>().Init(l.hwnd);
	TL::foreach<viewers_list, Common::__create_window__>()(&viewers, &l.hwnd);
	TL::foreach<viewers_list, __set_line__>()(&viewers, this);
	return 0;
}
//--------------------------------------------------------------------------------
namespace Common
{
	template<int N, class P>struct __in_rect__<CrossWindow::Line<N>, P>
	  : __in_rect_all__<CrossWindow::Line<N>, P, TL::EraseItem<CrossWindow::viewers_list, CrossViewer>::Result>{};
}
//----------------------------------------------------------------------------------------------
void CrossWindow::operator()(TMouseWell &l)
{
	TL::find<viewers_list, Common::__in_rect__>()(
		&viewers
		, &Common::__event_data__<TMouseWell, CrossWindow>(*this, l)
		);
}
//-----------------------------------------------------------------------------------
void CrossWindow::operator()(TCommand &l)
{
	EventDo(l);
}
//----------------------------------------------------------------------
wchar_t *CrossWindow::Title()
{
	return L"�������� ���������� ��������";
}
//--------------------------------------------------------------------------------------
//-----------todo test
unsigned xcolor[] = {
	0xffff0000
	, 0xff00ff00
	, 0xff0000ff
	, 0xffffff00
	, 0xff00ffff
};
	//---------------------todo test
bool CrossWindow::Draw(int offs, double &data, unsigned &color)
{
	data = (double)(rand() % 200) + 30;
	color = xcolor[rand() % 5];
	return offs < 512;
}
//----------------------------------------------------------------------------------------
