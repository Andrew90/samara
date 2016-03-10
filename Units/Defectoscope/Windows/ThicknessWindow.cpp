#include "stdafx.h"
#include "ThicknessWindow.h"
#include "MenuApi.h"
#include "ViewersMenu.hpp"
#include "Emptywindow.h"
#include "Common.h"
#include "DebugMess.h"
#include "App.h"
#include "LabelMessage.h"
//------------------------------------------------------------------------------------------------------------
ThicknessWindow::ThicknessWindow()
    : viewer(viewers.get<ThicknessViewer>())
{
	viewer.cursor.SetMouseMoveHandler(this, &ThicknessWindow::DrawCursor);	
}

void ThicknessWindow::operator()(TSize &l)
{
    RECT r;
	GetClientRect(l.hwnd, &r);

	Common::__data__ data = {Common::cross_window_height, r.right,  (r.bottom - Common::cross_window_height) / 4};
	TL::foreach<viewers_list, Common::__draw__>()(&viewers, &data);
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
	TL::foreach<line_list, Common::__set_border_color__>()(&viewers, this);
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

	template<template<class>class W, class P>struct __set_thresholds__<W<AboveBorder<Thickness> >, P>
	{
		typedef W<AboveBorder<Thickness> > O;
		void operator()(O *o, P *p)
		{
			o->value = Singleton<ThresholdsTable>::Instance().items.get<NominalBorder<Thickness> >().value[*p]
				+ Singleton<ThresholdsTable>::Instance().items.get<TL::Inner<O>::Result>().value[*p];
		}
	};

	template<template<class>class W, class P>struct __set_thresholds__<W<LowerBorder<Thickness> >, P>
	{
		typedef W<LowerBorder<Thickness> > O;
		void operator()(O *o, P *p)
		{
			o->value = Singleton<ThresholdsTable>::Instance().items.get<NominalBorder<Thickness> >().value[*p]
				- Singleton<ThresholdsTable>::Instance().items.get<TL::Inner<O>::Result>().value[*p];
		}
	};
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
//--------------------------------------------------------------------
void ThicknessWindow::operator()(TDestroy &l)
{
	SetWindowLongPtr(l.hwnd, GWLP_USERDATA, NULL);
	TL::foreach<viewers_list, Common::__destroy__window__>()(&viewers, (int *)0);
	delete this;
}
//----------------------------------------------------------------------
wchar_t *ThicknessWindow::Title()
{
	return L"Просмотр толщины";
}
//--------------------------------------------------------------------------------------
bool ThicknessWindow::DrawCursor(TMouseMove &l, VGraphics &g)
{
	int x, y;
	viewer.chart.CoordCell(l.x, l.y, x, y);	

	bool b = x < viewer.viewerData.currentOffsetZones;
	if(lastZone != x)
	{
		lastZone = x;
		if(b)
		{	
			char *s = StatusText(viewer.viewerData.commonStatus[x]);
			double valueMin = viewer.viewerData.bufferMin[x];
			double valueMax = viewer.viewerData.bufferMax[x];
			wsprintf(viewer.label.buffer, L"<ff>зона %d  датчик %d  минимум %s максимум %s  %S", 1 + x, 1 + y
				, Wchar_from<double, 5>(valueMin)()
				, Wchar_from<double, 5>(valueMax)()
				, s);
			TL::foreach<line_list, Common::__update__>()(&viewers, this);
		}
		else
		{
			viewer.label.buffer[0] = 0;
			TL::foreach<line_list, Common::__clr__>()(&viewers, this);
		}
	}
	viewer.label.Draw(g());
	drawZones = b;
	return b;
}
//-------------------------------------------------------------------------------------

