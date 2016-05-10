#pragma once
#include "DataViewer.h"
#include "LineViewer.h"
#include "typelist.hpp"
#include "ConstData.h"
#include "LineChart.hpp"
#include "AppBase.h"
#include "ThicknessViewer.h"

class XThicknessViewer: public ThicknessViewer
{
public:
	typedef ThicknessViewer Parent;
	void operator()(TRButtonDown &){}
};

class ThicknessWindow
{
public:
	typedef Thickness sub_type;
	int lastZone;
public:
	HWND hWnd;

	bool drawZones;
	
	typedef TL::CreateWapperNumList<Line, ThicknessWindow, 0, 7>::Result line_list;
	typedef TL::AppendList<
		    TL::MkTlst<XThicknessViewer>::Result
		    , line_list
	    >::Result viewers_list;
	TL::Factory<viewers_list> viewers;
	ThicknessViewer &viewer;
	ThicknessWindow();
	void operator()(TSize &);
	void operator()(TCommand &);
	void operator()(TGetMinMaxInfo &);
	unsigned operator()(TCreate &);
	void operator()(TMouseWell &);
	void operator()(TDestroy &);
	static wchar_t *Title();

	bool DrawCursor(TMouseMove &, VGraphics &);
};