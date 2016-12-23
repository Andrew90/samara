#include "stdafx.h"
#include "DataViewer.h"
#include "typelist.hpp"
#include "MedianFiltre.h"
#include "AppBase.h"
#include "Compute.h"
#include "App.h"


namespace
{
	static const int count_buffer = 3000;
	struct PTR
	{
		USPC7100_ASCANDATAHEADER *data[count_buffer];
	};
	struct Data
	{
		double data[count_buffer];
	};
}

DefectData::DefectData(int &filterWidth, bool &filterOn, double (&brak)[App::count_zones], double (&klass2)[App::count_zones])
	: medianFiltreWidth(filterWidth)
	, medianFiltreOn(filterOn)
	, brackThreshold(brak) 
	, klass2Threshold(klass2)
{}

void DefectData::Set(int zone_, int start, int stop, int channel, int offs, int maxOffs, USPC7100_ASCANDATAHEADER *s
	, void (*StatusZoneDefect)(int , double , int , double (&)[App::count_zones], double (&)[App::count_zones], char &))
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
				(*StatusZoneDefect)(offs, data[cnt], zone, brackThreshold, klass2Threshold, status[cnt]);
				if(++cnt >= dimention_of(data)) break;
			}
		}
	}
	else
	{
		MedianFiltre f;
		int offs = i - medianFiltreWidth * App::count_sensors;
		cnt = -medianFiltreWidth;
		if(offs < 0)
		{
			offs = 0;
			cnt = 0;
		}
	
		f.Clear(medianFiltreWidth);
		for(; i < stop; ++i)
		{
			if(channel == s[i].Channel)
			{
				double t = s[i].hdr.G1Amp;
				char st;
				(*StatusZoneDefect)(offs, t, zone, brackThreshold, klass2Threshold, st);
				
				int ret = f.Add(t, st, (void *)&s[i]);
				if(cnt >= 0)
				{
					if(StatusId<Clr<DeathZone>>() != st)
					{
						data[cnt] = f.buf[ret];
						scan[cnt] = (USPC7100_ASCANDATAHEADER *)f.data[ret];
						status[cnt] = f.status[ret];
					}
					else
					{
						data[cnt] = t;
						scan[cnt] = &s[i];
						status[cnt] = st;
					}
				}
				if(++cnt >= (int)dimention_of(data)) break;
			}
		}
	}
	count = cnt;
}
