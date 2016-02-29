#include "stdafx.h"
#include "CrossWindow.h"
#include "MenuApi.h"
#include "ViewersMenu.hpp"
#include "Emptywindow.h"
#include "Common.h"
#include "DebugMess.h"
#include "App.h"
#include "LabelMessage.h"

namespace 
{
	template<class O, class P>struct __set_border_color__
	{
		void operator()(O *o, P *p)
		{
			o->chart.items.get<LineViewer::BorderDefect>().color = 0xffff0000;
			o->owner = p;
		}
	};
}

CrossWindow::CrossWindow()
    : crossViewer(viewers.get<CrossViewer>())
	, border2Class(Singleton<ThresholdsTable>::Instance().items.get<Border2Class<Cross>>())
	, borderDefect(Singleton<ThresholdsTable>::Instance().items.get<BorderDefect<Cross>>())
{
	crossViewer.cursor.SetMouseMoveHandler(this, &CrossWindow::DrawCursor);
	TL::foreach<line_list, __set_border_color__>()(&viewers, this);
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
	template<int N, class P>struct __draw__<CrossWindow::Line<CrossWindow, N>, P>
	{
		typedef CrossWindow::Line<CrossWindow, N> O;
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
	//template<class O, class P>struct __set_line__
	//{
	//	void operator()(O *, P *){}
	//};
	//template<int N, class P>struct __set_line__<CrossWindow::Line<N>, P>
	//{
	//	typedef CrossWindow::Line<N> O;
	//	void operator()(O *o, P *p)
	//	{
	//		o->chart.items.get<BarSeries>().SetColorBarHandler(p, &CrossWindow::GetColorBar);
	//	}
	//};
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
	lastZone = -1;
	Menu<ViewersMenuCrossWindow::MainMenu>().Init(l.hwnd);
	TL::foreach<viewers_list, Common::__create_window__>()(&viewers, &l.hwnd);
	return 0;
}
//--------------------------------------------------------------------------------
namespace Common
{
	template<int N, class P>struct __in_rect__<CrossWindow::Line<CrossWindow, N>, P>
	  : __in_rect_all__<CrossWindow::Line<CrossWindow, N>, P, CrossWindow::line_list>{};
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
//-------------------------------------------------------------------------------------
namespace
{
	template<class O, class P>struct __drop__
	{
		void operator()(O *o, P *)
		{
			o->dataViewer.Drop();
		}
	};
}
void CrossWindow::operator()(TDestroy &)
{
	TL::foreach<CrossWindow::line_list, __drop__>()(&viewers, (int *)0);
}
//----------------------------------------------------------------------
wchar_t *CrossWindow::Title()
{
	return L"Просмотр поперечных дефектов";
}
//--------------------------------------------------------------------------------------
namespace
{
	template<class O, class P>struct __update__;
	template<int N, class P>struct __update__<CrossWindow::Line<CrossWindow, N>, P>
	{
		typedef CrossWindow::Line<CrossWindow, N> O;
		void operator()(O *o, P *p)
		{
			o->dataViewer.Do(p->lastZone);
			o->chart.maxAxesX = o->dataViewer.count;
			zprint("  count=%d\n", o->dataViewer.count);
			o->chart.items.get<LineViewer::Border2Class>().value = p->border2Class.value[p->lastZone];
			o->chart.items.get<LineViewer::BorderDefect>().value = p->borderDefect.value[p->lastZone];
			RepaintWindow(o->hWnd);
		}
	};
	template<class O, class P>struct __clr__;
	template<int N, class P>struct __clr__<CrossWindow::Line<CrossWindow, N>, P>
	{
		typedef CrossWindow::Line<CrossWindow, N> O;
		void operator()(O *o, P *p)
		{
			o->chart.maxAxesX = 0;
			o->chart.items.get<LineViewer::Border2Class>().value = 0;
			o->chart.items.get<LineViewer::BorderDefect>().value = 0;
			RepaintWindow(o->hWnd);
		}
	};
}
bool CrossWindow::DrawCursor(TMouseMove &l, VGraphics &g)
{
	int x, y;
	crossViewer.chart.CoordCell(l.x, l.y, x, y);	
	char *s = StatusText(crossViewer.viewerData.status[y][x]);
	wsprintf(crossViewer.label.buffer, L"<ff>зона %d  датчик %d    %S    ", 1 + x, 1 + y, s);
	crossViewer.label.Draw(g());
	bool b = x < crossViewer.viewerData.currentOffsetZones;
	if(lastZone != x)
	{
		lastZone = x;
		if(b)
		{			
			TL::foreach<line_list, __update__>()(&viewers, this);
		}
		else
		{
			TL::foreach<line_list, __clr__>()(&viewers, this);
		}
	}
	return b;
}
//-------------------------------------------------------------------------------------

