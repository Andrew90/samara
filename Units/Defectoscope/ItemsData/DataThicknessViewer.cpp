#include "stdafx.h"
#include "typelist.hpp"
#include "MedianFiltre.h"
#include "AppBase.h"
#include "Compute.h"
#include "DataViewer.h"


//namespace
//{
//	//static const int count_buffer = 3000;
//	//struct PTR
//	//{
//	//	USPC7100_ASCANDATAHEADER *data[count_buffer];
//	//};
//	//struct Data
//	//{
//	//	double data[count_buffer];
//	//};
//}

ThicknessData::ThicknessData(int &filterWidth, bool &filterOn
	, double (&aboveBorder)[App::zonesCount]
, double (&lowerBorder)[App::zonesCount]
, double (&nominalBorder)[App::zonesCount]
)
	: medianFiltreWidth(filterWidth)
	, medianFiltreOn(filterOn)
	, aboveBorder  (aboveBorder)
	, lowerBorder  (lowerBorder)
	, nominalBorder(nominalBorder)
{}

void ThicknessData::Set(int zone_, int start, int stop, int channel, int offs, int maxOffs, USPC7100_ASCANDATAHEADER *s)
{
	zone = zone_;
	stop += offs;
	int cnt = 0;
	if(stop > maxOffs) stop = maxOffs;
	int i = start + offs;
	if(!medianFiltreOn)
	{
		for(; i < stop; ++i)
		{
			if(channel == s[i].Channel)
			{
				data[cnt] = s[i].hdr.G1Amp;
				scan[cnt] = &s[i];
				StatusZoneThickness(offs, data[cnt], zone
					, aboveBorder  
					, lowerBorder  
					, nominalBorder
					, status[cnt]);
				if(++cnt >= dimention_of(data)) break;
			}
		}
	}
	else
	{
		MedianFiltre f;
		int offs = i - medianFiltreWidth * App::count_sensors;
		if(offs < 0) offs = 0;
		int z = 0;
		double tmp[dimention_of(f.buf)];
		USPC7100_ASCANDATAHEADER *sk[dimention_of(f.buf)];
		char stat[dimention_of(f.buf)];
		for(offs; offs < i; ++offs)
		{
			if(channel == s[offs].Channel)
			{
				tmp[z] = s[offs].hdr.G1Amp;
				sk[z] = &s[offs];
				
				StatusZoneThickness(offs, tmp[z], zone
					, aboveBorder  
					, lowerBorder  
					, nominalBorder
					, stat[z]);
				if(++z >= medianFiltreWidth) break;
			} 
		}
		
		f.Init(medianFiltreWidth, tmp);
		
		for(; i < stop; ++i)
		{
			if(channel == s[i].Channel)
			{
				double t = s[i].hdr.G1Amp;
				char st;
				StatusZoneThickness(offs, t, zone
					, aboveBorder  
					, lowerBorder  
					, nominalBorder
					, st);
				int ind = f.index % f.width;
				sk[ind] = &s[i];
				stat[ind] = st;
				int ret = f.Add(t);
				if(StatusId<Clr<DeathZone>>() != st)
				{
					data[cnt] = f.buf[ret];
					scan[cnt] = sk[ret];
					status[cnt] = stat[ret];
				}
				else
				{
					data[cnt] = t;
					scan[cnt] = &s[i];
					status[cnt] = st;
				}
				if(++cnt >= dimention_of(data)) break;
			}
		}
	}
	count = cnt;
}
