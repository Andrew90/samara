#pragma once
#include "CrossViewer.h"
#include "LineViewer.h"
#include "typelist.hpp"
#include "DataViewer.h"
#include "ConstData.h"
#include "LineChart.hpp"

class XCrossViewer: public CrossViewer
{
public:
	typedef CrossViewer Parent;
	void operator()(TRButtonDown &){}
};

class CrossWindow
{
public:
	typedef Cross sub_type;
	int lastZone;
public:
	HWND hWnd;
	bool drawZones;
	
	typedef TL::CreateWapperNumList<Line, CrossWindow, 0, 7>::Result line_list;
	typedef TL::AppendList<
		    TL::MkTlst<XCrossViewer>::Result
		    , line_list
	    >::Result viewers_list;
	TL::Factory<viewers_list> viewers;
	CrossViewer &viewer;
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