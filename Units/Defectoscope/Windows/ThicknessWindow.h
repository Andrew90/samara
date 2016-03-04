#pragma once
#include "DataViewer.h"
#include "LineViewer.h"
#include "typelist.hpp"
#include "ConstData.h"
#include "LineChart.hpp"
#include "AppBase.h"
#include "ThicknessViewer.h"

class ThicknessWindow
{
public:
	typedef Thickness sub_type;
	int lastZone;
public:
	HWND hWnd;
	//Border2Class<Thickness> &border2Class;
	//BorderDefect<Thickness> &borderDefect;

	AboveBorder  <Thickness> &aboveBorder  ;
	LowerBorder  <Thickness> &lowerBorder  ;
	NominalBorder<Thickness> &nominalBorder;



	double &minAxes, &maxAxes;
	bool drawZones;
	
	typedef TL::CreateWapperNumList<Line, ThicknessWindow, 0, 7>::Result line_list;
	typedef TL::AppendList<
		    TL::MkTlst<ThicknessViewer>::Result
		    , line_list
	    >::Result viewers_list;
	TL::Factory<viewers_list> viewers;
	ThicknessViewer &thicknessViewer;
	ThicknessWindow();
	void operator()(TSize &);
	void operator()(TCommand &);
	void operator()(TGetMinMaxInfo &);
	unsigned operator()(TCreate &);
	void operator()(TMouseWell &);
	static wchar_t *Title();

	bool DrawCursor(TMouseMove &, VGraphics &);
};