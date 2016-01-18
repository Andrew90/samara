#pragma once

class Chart;
class FixedGridSeries
{
	class TObj{};
	typedef bool(TObj::*Tptr)(unsigned, unsigned, double &, unsigned &);
	TObj *obj;
	Tptr SetColorBar;
//	double &dx;
//	double &dy;
	bool GetColorCellDefault(unsigned sensor, unsigned zone, double &data, unsigned &color);
public:
	int countZones;
	unsigned sensorCount;
	Chart &chart;
	FixedGridSeries(Chart &chart);
	void Draw();
	//void CoordCell(int mX, int mY, int &x, int &y);
	void OffsetToPixel(WORD &, WORD &, int , bool );
	template<class T>void SetColorCellHandler(T *t, bool(T::*ptr)(unsigned, int, double &, unsigned &))
	{
		obj = (TObj *)t;
		SetColorBar = (Tptr)ptr;
	}
};

class FixedGrid
{
public:
  Chart &chart;
  double &dX, &dY;
  FixedGrid(Chart &);
  void Draw();
};

class FixedLeftAxes
{
public:
  int leftOffset;
  Chart &chart;
  FixedLeftAxes(Chart &);
  void Draw();
};

class NoOffsetLeftAxes
{
public:
  int leftOffset;
  Chart &chart;
  NoOffsetLeftAxes(Chart &);
  void Draw();
};
