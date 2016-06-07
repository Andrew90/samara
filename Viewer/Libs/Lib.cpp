#include "StdAfx.h"
#include "Lib.h"

#include "CrossViewer.h"
#include "LongViewer.h"
#include "ThicknessViewer.h"
#include "message.h"

#include <memory>

#include "DebugMess.h"
#include "ConstData.h"

using namespace Gdiplus;

class CrossChart::Impl: public CrossViewer
{
public:
	void operator()(TSize &l);
};

void CrossChart::Impl::operator()(TSize &l)
{
	if(l.resizing == SIZE_MINIMIZED || 0 == l.Width || 0 == l.Height) return;	
		
	std::unique_ptr<Graphics> g(Graphics::FromHWND(l.hwnd));

	SolidBrush solidBrush(Color(0xffaaaaaa));
	g->FillRectangle(&solidBrush, 0, 0, 10, l.Height);   
	g->FillRectangle(&solidBrush, 0, 0, l.Width, 29);  
	
	
	chart.rect.right = l.Width;
	chart.rect.bottom = l.Height;
	chart.Draw(*g);
	label = L"<ff>Поперечные дефекты";
	label.Draw(*g);
}

CrossChart::CrossChart()
	: pimpl(new Impl)
{	
	AppBase().Init();
	ConstData::Init();	
}

CrossChart::~CrossChart()
{
	delete pimpl;
}

CrossChart *CrossChart::Create() 
{
	return new CrossChart;
}

void CrossChart::Destroy(CrossChart *x)
{
	delete x;
}

void CrossChart::Size(HWND h, unsigned resizing, unsigned short width, unsigned short height)
{
	TSize l = {h, 0, resizing, width, height};
	(*pimpl)(l);
}

void CrossChart::SetData(int channel , double *data)
{
	if((unsigned)channel < App::count_sensors)
	memmove(pimpl->viewerData.buffer[channel], data, sizeof(pimpl->viewerData.buffer[channel]));
}
void CrossChart::SetStatus(int channel, char *status)
{
	if((unsigned)channel < App::count_sensors)
	memmove(pimpl->viewerData.status[channel], status, sizeof(pimpl->viewerData.status[channel]));
}
void CrossChart::SetCountZones(int countZones)
{
	if((unsigned)countZones <= App::zonesCount)
	pimpl->viewerData.currentOffsetZones = countZones;
}
//-------------------------------------------------------------
class LongChart::Impl: public LongViewer
{
public:
	void operator()(TSize &l);
};

void LongChart::Impl::operator()(TSize &l)
{
	if(l.resizing == SIZE_MINIMIZED || 0 == l.Width || 0 == l.Height) return;	
		
	std::unique_ptr<Graphics> g(Graphics::FromHWND(l.hwnd));

	SolidBrush solidBrush(Color(0xffaaaaaa));
	g->FillRectangle(&solidBrush, 0, 0, 10, l.Height);   
	g->FillRectangle(&solidBrush, 0, 0, l.Width, 29);  
	
	
	chart.rect.right = l.Width;
	chart.rect.bottom = l.Height;
	chart.Draw(*g);
	label = L"<ff>Продольные дефекты";
	label.Draw(*g);
}

LongChart::LongChart()
	: pimpl(new Impl)
{}

LongChart::~LongChart()
{
	delete pimpl;
}

LongChart *LongChart::Create() 
{
	return new LongChart;
}

void LongChart::Destroy(LongChart *x)
{
	delete x;
}

void LongChart::Size(HWND h, unsigned resizing, unsigned short width, unsigned short height)
{
	TSize l = {h, 0, resizing, width, height};
	(*pimpl)(l);
}

void LongChart::SetData(int channel , double *data)
{
	if((unsigned)channel < App::count_sensors)
	memmove(pimpl->viewerData.buffer[channel], data, sizeof(pimpl->viewerData.buffer[channel]));
}

void LongChart::SetStatus(int channel, char *status)
{
	if((unsigned)channel < App::count_sensors)
	memmove(pimpl->viewerData.status[channel], status, sizeof(pimpl->viewerData.status[channel]));
}

void LongChart::SetCountZones(int countZones)
{	
	if((unsigned)countZones <= App::zonesCount)
	pimpl->viewerData.currentOffsetZones = countZones;
}
//------------------------------------------------------------------------------------------------
class ThicknessChart::Impl: public ThicknessViewer
{
public:
	void operator()(TSize &l);
};

void ThicknessChart::Impl::operator()(TSize &l)
{
	if(l.resizing == SIZE_MINIMIZED || 0 == l.Width || 0 == l.Height) return;	
		
	std::unique_ptr<Graphics> g(Graphics::FromHWND(l.hwnd));

	SolidBrush solidBrush(Color(0xffaaaaaa));
	g->FillRectangle(&solidBrush, 0, 0, 10, l.Height);   
	g->FillRectangle(&solidBrush, 0, 0, l.Width, 29);  
	
	
	chart.rect.right = l.Width;
	chart.rect.bottom = l.Height;
	chart.Draw(*g);
	label = L"<ff>Толщина";
	label.Draw(*g);
}

ThicknessChart::ThicknessChart()
	: pimpl(new Impl)
{}

ThicknessChart::~ThicknessChart()
{
	delete pimpl;
}

ThicknessChart *ThicknessChart::Create() 
{
	return new ThicknessChart;
}

void ThicknessChart::Destroy(ThicknessChart *x)
{
	delete x;
}

void ThicknessChart::Size(HWND h, unsigned resizing, unsigned short width, unsigned short height)
{
	TSize l = {h, 0, resizing, width, height};
	(*pimpl)(l);
}

void ThicknessChart::SetData(int channel , double *data)
{
	switch(channel)
	{
	case 0:
		memmove(pimpl->viewerData.bufferMin, data, sizeof(pimpl->viewerData.bufferMin));
		break;
	case 1:
		memmove(pimpl->viewerData.bufferMax, data, sizeof(pimpl->viewerData.bufferMax));
		break;
	}
}

void ThicknessChart::SetStatus(int channel, char *status)
{
	//switch(channel)
	//{
	//case 0:
	//	memmove(pimpl->viewerData.statusMin, status, sizeof(pimpl->viewerData.statusMin));
	//	dprint("status min %d\n", pimpl->viewerData.statusMin[99]);
	//	break;
	//case 1:
	//	memmove(pimpl->viewerData.statusMax, status, sizeof(pimpl->viewerData.statusMax));
	//	dprint("status max %d\n", pimpl->viewerData.statusMax[99]);
	//	break;
	//}
	
	memmove(pimpl->viewerData.commonStatus, status, sizeof(pimpl->viewerData.statusMin));
}

void ThicknessChart::SetCountZones(int countZones)
{	
	if((unsigned)countZones <= App::zonesCount)
	pimpl->viewerData.currentOffsetZones = countZones;
}

void ThicknessChart::SetMinMaxYAxes(double min, double max)
{
	pimpl->chart.minAxesY = min;
	pimpl->chart.maxAxesY = max;
}
//------------------------------------------------------------------------------------------------
void ChartsInit()
{
	AppBase().Init();
	ConstData::Init();
}

