#pragma once
#include "CrossViewer.h"
#include "LineViewer.h"
#include "typelist.hpp"
#include "DataViewer.h"

class CrossWindow
{
	int lastZone;
public:
	HWND hWnd;
	template<int N>struct Line: LineViewer
	{
		typedef LineViewer Parent;
		DataViewer<Cross, N> dataViewer;
		Line()
		{
			chart.items.get<BarSeries>().SetColorBarHandler(this, &Line<N>::GetColorBar);
		}
		bool GetColorBar(int offs, double &data, unsigned &color)
		{
			 if(NULL != dataViewer.data && offs < dataViewer.count)
			 {
				 color = 0xff00ff00;
				 data = dataViewer.data[offs];
				 return true;
			 }
			 return false;
		}
	};
	typedef TL::AppendList<
		    TL::MkTlst<CrossViewer>::Result
		    , TL::CreateNumList<Line, 0, 7>::Result
	    >::Result viewers_list;
	TL::Factory<viewers_list> viewers;
	CrossViewer &crossViewer;
	CrossWindow();
	void operator()(TSize &);
	void operator()(TCommand &);
	void operator()(TGetMinMaxInfo &);
	unsigned operator()(TCreate &);
	void operator()(TMouseWell &);
	void operator()(TDestroy &);
	static wchar_t *Title();

	bool DrawCursor(TMouseMove &, VGraphics &);
};