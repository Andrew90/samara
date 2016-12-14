#include "stdafx.h"
#include "typelist.hpp"
#include "MedianFiltre.h"
#include "AppBase.h"
#include "Compute.h"
#include "DataViewer.h"

ThicknessData::ThicknessData(int &filterWidth, bool &filterOn
	, double (&aboveBorder)[App::count_zones]
, double (&lowerBorder)[App::count_zones]
, double (&nominalBorder)[App::count_zones]
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
	ItemData<Thickness> &d = Singleton<ItemData<Thickness> >::Instance();
	if(!medianFiltreOn)
	{
		for(; i < stop; ++i)
		{
			if(channel == s[i].Channel)
			{
				data[cnt] = 2.5e-6 * s[i].hdr.G1Tof * d.param[channel].get<gate1_TOF_WT_velocity>().value;
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
				tmp[z] = 2.5e-6 * s[offs].hdr.G1Tof * d.param[channel].get<gate1_TOF_WT_velocity>().value;
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
				double t = 2.5e-6 * s[i].hdr.G1Tof *d.param[channel].get<gate1_TOF_WT_velocity>().value;
				char st;
				StatusZoneThickness(offs, t, zone
					, aboveBorder  
					, lowerBorder  
					, nominalBorder
					, st);
				int ind = f.index % f.width;
				sk[ind] = &s[i];
				stat[ind] = st;
				if(0 == t) t = 999999;
				int ret = f.Add(t);
				//if(StatusId<Clr<DeathZone>>() != st)
				{
					t = f.buf[ret];
					if(999999 == t) t = 0;
					
					data[cnt] = t;//f.buf[ret];
					scan[cnt] = sk[ret];
					status[cnt] = stat[ret];
				}
				//else
				//{
				//	data[cnt] = t;
				//	scan[cnt] = &s[i];
				//	status[cnt] = st;
				//}
				if(++cnt >= dimention_of(data)) break;
			}
		}
	}
	count = cnt;
}
