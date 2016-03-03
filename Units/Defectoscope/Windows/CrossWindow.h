#pragma once
#include "CrossViewer.h"
#include "LineViewer.h"
#include "typelist.hpp"
#include "DataViewer.h"
#include "ConstData.h"

class CrossWindow
{
public:
	int lastZone;
public:
	HWND hWnd;
	Border2Class<Cross> &border2Class;
	BorderDefect<Cross> &borderDefect;
	double &minAxes, &maxAxes;
	bool drawZones;
	template<class T, int N>struct Line: LineViewer
	{
		typedef LineViewer Parent;
		T *owner;		
		DataViewer<Cross, N> dataViewer;
		Line()
		{
			chart.items.get<BarSeries>().SetColorBarHandler(this, &Line::GetColorBar);
			cursor.SetMouseMoveHandler(this, &Line<T, N>::CursorDraw);
		}	
		bool GetColorBar(int offs, double &data, unsigned &color)
		{
			if(NULL != dataViewer.data && offs < dataViewer.count)
			{
				data = dataViewer.data[offs];
				color = ConstData::ZoneColor(dataViewer.status[offs]);
				return true;
			}
			return false;
		}
		
		bool CursorDraw(TMouseMove &l, VGraphics &g)	  
		{	
			if(owner->drawZones)
			{
				double valY = chart.items.get<BarSeries>().ValueY(offsetX);
				char *s = StatusText(dataViewer.status[offsetX]);
				wsprintf(label.buffer, L"<ff>Датчик <ff0000>%d <ff>смещение %d  величина %s   %S     ", 1 + N, offsetX, Wchar_from<double, 5>(valY)(), s);
			}
			else
			{
				label.buffer[0] = 0;
			}
			label.Draw(g());
			return true;
		}
	};
	
	typedef TL::CreateWapperNumList<Line, CrossWindow, 0, 7>::Result line_list;
	typedef TL::AppendList<
		    TL::MkTlst<CrossViewer>::Result
		    , line_list
	    >::Result viewers_list;
	TL::Factory<viewers_list> viewers;
	CrossViewer &crossViewer;
	CrossWindow();
	void operator()(TSize &);
	void operator()(TCommand &);
	void operator()(TGetMinMaxInfo &);
	unsigned operator()(TCreate &);
	void operator()(TMouseWell &);
	static wchar_t *Title();

	bool DrawCursor(TMouseMove &, VGraphics &);
};