#pragma once
#include "CrossViewer.h"
#include "LineViewer.h"
#include "typelist.hpp"
#include "DataViewer.h"

class CrossWindow
{
public:
	int lastZone;
public:
	HWND hWnd;
	Border2Class<Cross> &border2Class;
	BorderDefect<Cross> &borderDefect;
	template<class T, int N>struct Line: LineViewer
	{
		typedef LineViewer Parent;
		T *owner;
		DataViewer<Cross, N> dataViewer;
		Line()
			: LineViewer(N)
		{
			chart.items.get<BarSeries>().SetColorBarHandler(this, &Line::GetColorBar);
		}	
		bool Line::GetColorBar(int offs, double &data, unsigned &color)
		{
			if(NULL != dataViewer.data && offs < dataViewer.count)
			{
				 data = dataViewer.data[offs];
				 if(owner->borderDefect.value[dataViewer.zone] < data)
				 {
					 color = 0xffff0000;
				 }
				 else if(owner->border2Class.value[dataViewer.zone] < data)
				 {
					 color = 0xffffff00;
				 }
				 else
				 {
					 color = 0xff00ff00;
				 }
				 return true;
			}
			return false;
		}
		void operator()(TRButtonDown &l)
		{
			//PopupMenu<ContextMenuCrossWindow::items_list>::Do(l.hwnd, l.hwnd);
			zprint(" >>>>> Добавить меню <<<\n");
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
	void operator()(TDestroy &);
	static wchar_t *Title();

	bool DrawCursor(TMouseMove &, VGraphics &);
};