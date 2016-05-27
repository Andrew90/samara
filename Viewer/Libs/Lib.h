#pragma once

class GridChart
{
	struct Impl;
	Impl *pimpl;
	GridChart();
	~GridChart();
public:	
	static GridChart *Create();
	static void Destroy(GridChart *);

	void Size(HWND h, unsigned msg, unsigned resizing, unsigned short width, unsigned short height);
	void Paint(HWND h, unsigned msg, HDC hdc);
};
