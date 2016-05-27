#include "StdAfx.h"
#include "Lib.h"

#include "CrossViewer.h"
#include "message.h"

struct GridChart::Impl: public CrossViewer
{
};

GridChart::GridChart()
	: pimpl(new Impl)
{
}

GridChart::~GridChart()
{
	delete pimpl;
}

GridChart *GridChart::Create() 
{
	return new GridChart;
}

void GridChart::Destroy(GridChart *x)
{
	delete x;
}

void GridChart::Size(HWND h, unsigned msg, unsigned resizing, unsigned short width, unsigned short height)
{
	TSize l = {h, msg, resizing, width, height};
	(*pimpl)(l);
}

void GridChart::Paint(HWND h, unsigned msg, HDC hdc)
{
	TPaint l = {h, msg, hdc, 0};
	(*pimpl)(l);
}