#include "stdafx.h"
#include "Gate.h"
#include "Chart.h"

using namespace Gdiplus;
Gate::Gate(Chart &c)
	: chart(c)
	, color(0xff0000ff)
{
}
void Gate::Draw()
{
	Color col(color);
	Pen pen(col, 1);
	double dX = (double)(chart.rect.right - chart.rect.left - chart.offsetAxesLeft - chart.offsetAxesRight) / (chart.maxAxesX - chart.minAxesX);

	double xx =  chart.rect.left + chart.offsetAxesLeft + (x - chart.minAxesX) * dX;

	double width__ = width * dX;

	double dY = (double) (chart.rect.bottom - chart.rect.top - chart.offsetAxesBottom - chart.offsetAxesTop) / (chart.maxAxesY - chart.minAxesY);
    double yy = chart.rect.bottom - chart.offsetAxesBottom - (y - chart.minAxesY) * dY;

	chart.g->DrawLine(&pen, (int)xx, (int)yy, (int)(xx + width__), (int)yy);
}
