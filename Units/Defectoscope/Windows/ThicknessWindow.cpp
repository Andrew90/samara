#include "stdafx.h"
#include "ThicknessWindow.h"
#include "MenuApi.h"
#include "ViewersMenu.hpp"
#include "Emptywindow.h"
#include "Common.h"
#include "DebugMess.h"
#include "App.h"
#include "LabelMessage.h"

namespace 
{
#pragma message("Дописать цвета порогов")
	template<class O, class P>struct __set_border_color__
	{
		void operator()(O *o, P *p)
		{
			//o->chart.items.get<LineViewer::BorderDefect>().color = 0xffff0000;
			o->owner = p;
			o->chart->minAxesY = p->minAxes;
			o->chart->maxAxesY = p->maxAxes;
		}
	};
}

ThicknessWindow::ThicknessWindow()
    : thicknessViewer(viewers.get<ThicknessViewer>())
	, aboveBorder  (Singleton<ThresholdsTable>::Instance().items.get<AboveBorder  <Thickness>>())
	, lowerBorder  (Singleton<ThresholdsTable>::Instance().items.get<LowerBorder  <Thickness>>())
	, nominalBorder(Singleton<ThresholdsTable>::Instance().items.get<NominalBorder<Thickness>>())
	, minAxes(Singleton<AxesTable>::Instance().items.get<AxesYMin<Thickness> >().value)
	, maxAxes(Singleton<AxesTable>::Instance().items.get<AxesYMax<Thickness> >().value)

{
	thicknessViewer.cursor.SetMouseMoveHandler(this, &ThicknessWindow::DrawCursor);	
}
namespace
{
	static const int cross_window_height = 200;
	struct __data__
	{
		int y, width, height;
	};
	template<class O, class P>struct __draw__;
	template<class P>struct __draw__<ThicknessViewer, P>
	{
		typedef ThicknessViewer O;
		void operator()(O *o, P *p)
		{
			 MoveWindow(o->hWnd, 0, 0, p->width, cross_window_height, TRUE);
		}
	};
	template<int N, class P>struct __draw__<Line<ThicknessWindow, N>, P>
	{
		typedef Line<ThicknessWindow, N> O;
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
void ThicknessWindow::operator()(TSize &l)
{
    RECT r;
	GetClientRect(l.hwnd, &r);

	__data__ data = {cross_window_height, r.right,  (r.bottom - cross_window_height) / 4};
	TL::foreach<viewers_list, __draw__>()(&viewers, &data);
}
void ThicknessWindow::operator()(TGetMinMaxInfo &m)
{
	if(NULL != m.pMinMaxInfo)
	{
		m.pMinMaxInfo->ptMinTrackSize.x = 600;
		m.pMinMaxInfo->ptMinTrackSize.y = 600;
	}	
}
VIEWERS_MENU(ThicknessWindow)
unsigned ThicknessWindow::operator()(TCreate &l)
{
	TL::foreach<line_list, __set_border_color__>()(&viewers, this);
	lastZone = -1;
	Menu<ViewersMenuThicknessWindow::MainMenu>().Init(l.hwnd);
	TL::foreach<viewers_list, Common::__create_window__>()(&viewers, &l.hwnd);
	return 0;
}
//--------------------------------------------------------------------------------
namespace Common
{
	template<int N, class P>struct __in_rect__<Line<ThicknessWindow, N>, P>
	  : __in_rect_all__<Line<ThicknessWindow, N>, P, ThicknessWindow::line_list>{};
}
//----------------------------------------------------------------------------------------------
void ThicknessWindow::operator()(TMouseWell &l)
{
	TL::find<viewers_list, Common::__in_rect__>()(
		&viewers
		, &Common::__event_data__<TMouseWell, ThicknessWindow>(*this, l)
		);
}
//-----------------------------------------------------------------------------------
void ThicknessWindow::operator()(TCommand &l)
{
	EventDo(l);
}
//----------------------------------------------------------------------
wchar_t *ThicknessWindow::Title()
{
	return L"Просмотр толщины";
}
//--------------------------------------------------------------------------------------
namespace
{
#pragma message("дописать ")
	template<class O, class P>struct __update__;
	template<int N, class P>struct __update__<Line<ThicknessWindow, N>, P>
	{
		typedef Line<ThicknessWindow, N> O;
		void operator()(O *o, P *p)
		{
			o->dataViewer.Do(p->lastZone);
			o->chart->maxAxesX = o->dataViewer.count;
			//((O::TChart *)o->chart)->items.get<LineViewer::Border2Class>().value = p->border2Class.value[p->lastZone];
			//((O::TChart *)o->chart)->items.get<LineViewer::BorderDefect>().value = p->borderDefect.value[p->lastZone];
			RepaintWindow(o->hWnd);
		}
	};
	template<class O, class P>struct __clr__;
	template<int N, class P>struct __clr__<Line<ThicknessWindow, N>, P>
	{
		typedef Line<ThicknessWindow, N> O;
		void operator()(O *o, P *p)
		{
			o->chart->maxAxesX = 0;
			//o->chart.items.get<LineViewer::Border2Class>().value = 0;
			//o->chart.items.get<LineViewer::BorderDefect>().value = 0;
			RepaintWindow(o->hWnd);
		}
	};
}
bool ThicknessWindow::DrawCursor(TMouseMove &l, VGraphics &g)
{
	int x, y;
	thicknessViewer.chart.CoordCell(l.x, l.y, x, y);	
	
	bool b = x < thicknessViewer.viewerData.currentOffsetZones;
	if(lastZone != x)
	{
		lastZone = x;
		if(b)
		{	
			char *s = StatusText(thicknessViewer.viewerData.status[x]);
			double valueMin = thicknessViewer.viewerData.bufferMin[x];
			double valueMax = thicknessViewer.viewerData.bufferMax[x];
			wsprintf(thicknessViewer.label.buffer, L"<ff>зона %d  датчик %d  минимум %s максимум %s  %S", 1 + x, 1 + y
				, Wchar_from<double, 5>(valueMin)()
				, Wchar_from<double, 5>(valueMax)()
				, s);
			TL::foreach<line_list, __update__>()(&viewers, this);
		}
		else
		{
			thicknessViewer.label.buffer[0] = 0;
			TL::foreach<line_list, __clr__>()(&viewers, this);
		}
		thicknessViewer.label.Draw(g());
	}
	drawZones = b;
	return b;
}
//-------------------------------------------------------------------------------------

