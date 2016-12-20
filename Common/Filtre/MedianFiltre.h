#pragma once 

struct MedianFiltre
{
	int index;
	int width;
	int medianIndex;
	double buf[16];
	int status[16];
	int ind[16];
	int Init(int , double *);
	int Add(double);
	int Add(double, int);
	void Clear(int);
	void SetVal(int, double );
public:
	int indexX;
	double bufX[16];
	int indX[16];
	int AddX(double);
};