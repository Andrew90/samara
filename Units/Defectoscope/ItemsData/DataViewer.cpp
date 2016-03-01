#include "stdafx.h"
#include "DataViewer.h"
#include "typelist.hpp"
#include "MedianFiltre.h"
#include "AppBase.h"


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

DefectData::DefectData()
	: medianFiltreWidth(Singleton<MedianFiltreTable>::Instance().items.get<MedianFiltreWidth>().value)
	, medianFiltreOn(Singleton<MedianFiltreTable>::Instance().items.get<MedianFiltreOn>().value)
{}

void DefectData::Set(int z, int start, int stop, int channel, int offs, int maxOffs, USPC7100_ASCANDATAHEADER *s)
{
	zone = z;
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
				data[cnt] = s[i].hdr.G2Amp;
				scan[cnt] = &s[i];
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
		for(offs; offs < i; ++offs)
		{
			if(channel == s[offs].Channel)
			{
				tmp[z] = s[offs].hdr.G2Amp;
				sk[z] = &s[offs];
				if(++z >= medianFiltreWidth) break;
			} 
		}
		
		f.Init(medianFiltreWidth, tmp);
		
		for(; i < stop; ++i)
		{
			if(channel == s[i].Channel)
			{
				sk[f.index % f.width] = &s[i];
				int ret = f.Add(s[i].hdr.G2Amp);
				data[cnt] = f.buf[ret];
				scan[cnt] = sk[ret];
				if(++cnt >= dimention_of(data)) break;
			}
		}
	}
	count = cnt;
}
