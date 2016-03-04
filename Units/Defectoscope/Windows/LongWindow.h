#pragma once
#include "LongViewer.h"
#include "LineViewer.h"
#include "typelist.hpp"
#include "DataViewer.h"
#include "ConstData.h"
#include "LineChart.hpp"

class LongWindow
{
public:
	typedef Long sub_type;
	int lastZone;
public:
	HWND hWnd;
	Border2Class<Long> &border2Class;
	BorderDefect<Long> &borderDefect;
	double &minAxes, &maxAxes;
	bool drawZones;
	
	typedef TL::CreateWapperNumList<Line, LongWindow, 0, 7>::Result line_list;
	typedef TL::AppendList<
		    TL::MkTlst<LongViewer>::Result
		    , line_list
	    >::Result viewers_list;
	TL::Factory<viewers_list> viewers;
	LongViewer &longViewer;
	LongWindow();
	void operator()(TSize &);
	void operator()(TCommand &);
	void operator()(TGetMinMaxInfo &);
	unsigned operator()(TCreate &);
	void operator()(TMouseWell &);
	static wchar_t *Title();

	bool DrawCursor(TMouseMove &, VGraphics &);
};