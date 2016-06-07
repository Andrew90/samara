#pragma once

class CrossChart
{
	class Impl;
	Impl *pimpl;
	CrossChart();
	~CrossChart();
public:	
	static CrossChart *Create();
	static void Destroy(CrossChart *);

	void Size(HWND h, unsigned resizing, unsigned short width, unsigned short height);
	void SetData(int, double *);
	void SetStatus(int, char *);
	void SetCountZones(int);
};

class LongChart
{
	class Impl;
	Impl *pimpl;
	LongChart();
	~LongChart();
public:	
	static LongChart *Create();
	static void Destroy(LongChart *);

	void Size(HWND h, unsigned resizing, unsigned short width, unsigned short height);
	void SetData(int, double *);
	void SetStatus(int, char *);
	void SetCountZones(int);
};

class ThicknessChart
{
	class Impl;
	Impl *pimpl;
	ThicknessChart();
	~ThicknessChart();
public:	
	static ThicknessChart *Create();
	static void Destroy(ThicknessChart *);

	void Size(HWND h, unsigned resizing, unsigned short width, unsigned short height);
	void SetData(int, double *);
	void SetStatus(int, char *);
	void SetCountZones(int);
	void SetMinMaxYAxes(double, double);
};

void ChartsInit();

void RepaintWindow(HWND h);


