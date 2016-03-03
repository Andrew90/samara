#include "stdafx.h"
#include "LongWindow.h"
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
			o->chart.minAxesY = p->minAxes;
			o->chart.maxAxesY = p->maxAxes;
		}
	};
}

LongWindow::LongWindow()
    : longViewer(viewers.get<LongViewer>())
	, border2Class(Singleton<ThresholdsTable>::Instance().items.get<Border2Class<Long>>())
	, borderDefect(Singleton<ThresholdsTable>::Instance().items.get<BorderDefect<Long>>())
	, minAxes(Singleton<AxesTable>::Instance().items.get<AxesYMin<Long> >().value)
	, maxAxes(Singleton<AxesTable>::Instance().items.get<AxesYMax<Long> >().value)

{
	longViewer.cursor.SetMouseMoveHandler(this, &LongWindow::DrawCursor);	
}
namespace
{
	static const int cross_window_height = 200;
	struct __data__
	{
		int y, width, height;
	};
	template<class O, class P>struct __draw__;
	template<class P>struct __draw__<LongViewer, P>
	{
		typedef LongViewer O;
		void operator()(O *o, P *p)
		{
			 MoveWindow(o->hWnd, 0, 0, p->width, cross_window_height, TRUE);
		}
	};
	template<int N, class P>struct __draw__<Line<LongWindow, N>, P>
	{
		typedef Line<LongWindow, N> O;
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
void LongWindow::operator()(TSize &l)
{
    RECT r;
	GetClientRect(l.hwnd, &r);

	__data__ data = {cross_window_height, r.right,  (r.bottom - cross_window_height) / 4};
	TL::foreach<viewers_list, __draw__>()(&viewers, &data);
}
void LongWindow::operator()(TGetMinMaxInfo &m)
{
	if(NULL != m.pMinMaxInfo)
	{
		m.pMinMaxInfo->ptMinTrackSize.x = 600;
		m.pMinMaxInfo->ptMinTrackSize.y = 600;
	}	
}
VIEWERS_MENU(LongWindow)
unsigned LongWindow::operator()(TCreate &l)
{
	TL::foreach<line_list, __set_border_color__>()(&viewers, this);
	lastZone = -1;
	Menu<ViewersMenuLongWindow::MainMenu>().Init(l.hwnd);
	TL::foreach<viewers_list, Common::__create_window__>()(&viewers, &l.hwnd);
	return 0;
}
//--------------------------------------------------------------------------------
namespace Common
{
	template<int N, class P>struct __in_rect__<Line<LongWindow, N>, P>
	  : __in_rect_all__<Line<LongWindow, N>, P, LongWindow::line_list>{};
}
//----------------------------------------------------------------------------------------------
void LongWindow::operator()(TMouseWell &l)
{
	TL::find<viewers_list, Common::__in_rect__>()(
		&viewers
		, &Common::__event_data__<TMouseWell, LongWindow>(*this, l)
		);
}
//-----------------------------------------------------------------------------------
void LongWindow::operator()(TCommand &l)
{
	EventDo(l);
}
//----------------------------------------------------------------------
wchar_t *LongWindow::Title()
{
	return L"Просмотр поперечных дефектов";
}
//--------------------------------------------------------------------------------------
namespace
{
	template<class O, class P>struct __update__;
	template<int N, class P>struct __update__<Line<LongWindow, N>, P>
	{
		typedef Line<LongWindow, N> O;
		void operator()(O *o, P *p)
		{
			o->dataViewer.Do(p->lastZone);
			o->chart.maxAxesX = o->dataViewer.count;
			o->chart.items.get<LineViewer::Border2Class>().value = p->border2Class.value[p->lastZone];
			o->chart.items.get<LineViewer::BorderDefect>().value = p->borderDefect.value[p->lastZone];
			RepaintWindow(o->hWnd);
		}
	};
	template<class O, class P>struct __clr__;
	template<int N, class P>struct __clr__<Line<LongWindow, N>, P>
	{
		typedef Line<LongWindow, N> O;
		void operator()(O *o, P *p)
		{
			o->chart.maxAxesX = 0;
			o->chart.items.get<LineViewer::Border2Class>().value = 0;
			o->chart.items.get<LineViewer::BorderDefect>().value = 0;
			RepaintWindow(o->hWnd);
		}
	};
}
bool LongWindow::DrawCursor(TMouseMove &l, VGraphics &g)
{
	int x, y;
	longViewer.chart.CoordCell(l.x, l.y, x, y);	
	
	bool b = x < longViewer.viewerData.currentOffsetZones;
	if(lastZone != x)
	{
		lastZone = x;
		if(b)
		{	
			char *s = StatusText(longViewer.viewerData.status[y][x]);
			double value = longViewer.viewerData.buffer[y][x];
			wsprintf(longViewer.label.buffer, L"<ff>зона %d  датчик %d  %s  %S", 1 + x, 1 + y, Wchar_from<double, 5>(value)(), s);
			TL::foreach<line_list, __update__>()(&viewers, this);
		}
		else
		{
			longViewer.label.buffer[0] = 0;
			TL::foreach<line_list, __clr__>()(&viewers, this);
		}
		longViewer.label.Draw(g());
	}
	drawZones = b;
	return b;
}
//-------------------------------------------------------------------------------------

