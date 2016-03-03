#pragma once 

struct MedianFiltre
{
	int index;
	int width;
	int medianIndex;
	double buf[16];
	int ind[16];
	int Init(int , double *);
	int Add(double);
	void Clear(int);
};