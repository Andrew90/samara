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
	template<class T>struct ColorThreshold
	{
		typedef typename T::__template_must_be_overridded__ noused;
	};

	template<template<class>class W, class X>struct ColorThreshold<W<BorderDefect<X> > >
	{
		typedef Defect Result;
	};
	template<template<class>class W, class X>struct ColorThreshold<W<Border2Class<X> > >
	{
		typedef Treshold2Class Result;
	};

	template<class O, class P>struct __set_color__
	{
			void operator()(O *o, P *p)
			{
				o->color = Singleton<ColorTable>::Instance().items.get<typename ColorThreshold<O>::Result>().value;
			}
	};
	template<class O, class P>struct __set_border_color__
	{
		void operator()(O *o, P *p)
		{
			typedef TL::SelectWapper<typename O::TChart::items_list, Border>::Result lst;
			TL::foreach<lst, __set_color__>()(&((typename O::TChart *)o->chart)->items, p);
			o->owner = p;
			o->chart->minAxesY = Singleton<AxesTable>::Instance().items.get<AxesYMin<typename P::sub_type> >().value;
			o->chart->maxAxesY = Singleton<AxesTable>::Instance().items.get<AxesYMax<typename P::sub_type> >().value;
		}
	};
	template<class O, class P>struct __set_thresholds__
	{
		void operator()(O *o, P *p)
		{
			o->value = Singleton<ThresholdsTable>::Instance().items.get<TL::Inner<O>::Result>().value[*p];
		}
	};
	template<class O, class P>struct __update__;
	template<int N, class P>struct __update__<Line<CrossWindow, N>, P>
	{
		typedef Line<CrossWindow, N> O;
		void operator()(O *o, P *p)
		{
			o->dataViewer.Do(p->lastZone);
			o->chart->maxAxesX = o->dataViewer.count;
			typedef TL::SelectWapper<typename O::TChart::items_list, Border>::Result lst;
			TL::foreach<lst, __set_thresholds__>()(&((typename O::TChart *)o->chart)->items, &p->lastZone);
			RepaintWindow(o->hWnd);
		}
	};
	template<class O, class P>struct __clr__;
	template<int N, class P>struct __clr__<Line<CrossWindow, N>, P>
	{
		typedef Line<CrossWindow, N> O;
		void operator()(O *o, P *p)
		{
			o->chart->maxAxesX = 0;
			RepaintWindow(o->hWnd);
		}
	};

	static const int cross_window_height = 200;
	struct __data__
	{
		int y, width, height;
	};
	
	template<class O, class P>struct __draw__
	{
		void operator()(O *o, P *p)
		{
			 MoveWindow(o->hWnd, 0, 0, p->width, cross_window_height, TRUE);
		}
	};
	template<class W, int N, class P>struct __draw__<Line<W, N>, P>
	{
		typedef Line<W, N> O;
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

CrossWindow::CrossWindow()
    : viewer(viewers.get<CrossViewer>())
{
	viewer.cursor.SetMouseMoveHandler(this, &CrossWindow::DrawCursor);	
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
	TL::foreach<line_list, __set_border_color__>()(&viewers, this);
	lastZone = -1;
	Menu<ViewersMenuCrossWindow::MainMenu>().Init(l.hwnd);
	TL::foreach<viewers_list, Common::__create_window__>()(&viewers, &l.hwnd);
	return 0;
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
	return L"Просмотр поперечных дефектов";
}
//--------------------------------------------------------------------------------------
bool CrossWindow::DrawCursor(TMouseMove &l, VGraphics &g)
{
	int x, y;
	viewer.chart.CoordCell(l.x, l.y, x, y);	
	
	bool b = x < viewer.viewerData.currentOffsetZones;
	if(lastZone != x)
	{
		lastZone = x;
		if(b)
		{	
			char *s = StatusText(viewer.viewerData.status[y][x]);
			double value = viewer.viewerData.buffer[y][x];
			wsprintf(viewer.label.buffer, L"<ff>зона %d  датчик %d  %s  %S", 1 + x, 1 + y, Wchar_from<double, 5>(value)(), s);
			TL::foreach<line_list, __update__>()(&viewers, this);
		}
		else
		{
			viewer.label.buffer[0] = 0;
			TL::foreach<line_list, __clr__>()(&viewers, this);
		}
		viewer.label.Draw(g());
	}
	drawZones = b;
	return b;
}
//-------------------------------------------------------------------------------------

