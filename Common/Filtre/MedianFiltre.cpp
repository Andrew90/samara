#include "stdafx.h"
#include "MedianFiltre.h"
#include <windows.h>

int MedianFiltre::Init(int width_, double *d)
{
	if(width_ > 2 && width_ < 16)
	{
	    width_ |= 1;

		index = width_;

		medianIndex = width_ / 2;
		width = width_;
		ZeroMemory(ind, sizeof(ind));
		memcpy(buf, d, sizeof(double) * width_);
		for(int i = 0; i < width_ - 1; ++i)
		{
		   double x = buf[i];
		   for(int j = 1 + i; j < width_; ++j)
		   {
			   if(x > buf[j]) ++ind[i]; else ++ind[j];
		   }
		}

		for(int i = 0; i < width_; ++i)
			if(medianIndex == ind[i]) return i;
	}
	return 0;
}

int MedianFiltre::Add(double d)
{
	int index_ = index % width;
	++index;
	double x = buf[index_];
	for(int i = 0; i < width; ++i)
	{
		if(x < buf[i]) --ind[i];
	}

	int cnt = 0;
	buf[index_] = d;

	for(int i = 0; i < width; ++i)
	{
		if(i != index_)
		{
			if(d > buf[i]) ++cnt; else ++ind[i];
		}
	}
	ind[index_] = cnt;
	//for(int i = index - width, last = index; i < last; ++i)//не стирать
	//{														 //не стирать
	//	int k = i % width;									 //не стирать
	//	if(medianIndex == ind[k]) 							 //не стирать
	//		return i;										 //не стирать
	//}														 //не стирать
	//														 //не стирать
	//return index - 1;										 //не стирать

	for(int i = 0; i < width; ++i)
	{
		if(medianIndex == ind[i]) return i;
	}
	return 0;
}
//--------------------------------------------------------------------
void MedianFiltre::Clear(int width_)
{
	medianIndex = width_ / 2;
	index = 0;
	width = width_;
	ZeroMemory(buf, sizeof(buf));
}