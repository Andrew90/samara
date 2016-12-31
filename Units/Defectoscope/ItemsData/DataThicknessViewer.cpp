#include "stdafx.h"
#include "templates/typelist.hpp"
#include "Filtre\MedianFiltre.h"
#include "App/AppBase.h"
#include "Compute/Compute.h"
#include "ItemsData\DataViewer.h"
#include "MessageText\LabelMessage.h"

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
	double brackStrobe = Singleton<BrackStrobe2Table>::Instance().items.get<BrakStrobe2<Thickness>>().value;
	double nominal = Singleton<ThresholdsTable>::Instance().items.get<BorderNominal<Thickness>>().value[zone_];
	int ret = 0;
	if(!medianFiltreOn)
	{		
		for(; i < stop; ++i)
		{
			if(channel == s[i].Channel)
			{
				static const int Status = TL::IndexOf<ColorTable::items_list, Clr<BrakStrobe2<Thickness>>>::value;
				char st = StatusId<Clr<Undefined>>();
				data[cnt] = 0;
				bool errorStrobe = true;
				if(s[i].hdr.G1Tof)
				{
					double val = 2.5e-6 * s[i].hdr.G1Tof * d.param[channel].get<gate1_TOF_WT_velocity>().value;
					data[cnt] = val;
					if(s[i].hdr.G2Tof)
					{
						double val2 = 2.5e-6 * s[i].hdr.G2Tof * d.param[channel].get<gate2_TOF_WT_velocity>().value;
						double t = val - val2;
						if(t > brackStrobe)
						{
							st = Status;
						}
					}
				}
				scan[cnt] = &s[i];
				if(Status == st)
				{
					status[cnt] = st;
				}
				else
				{					
					StatusZoneThickness(offs, data[cnt], zone
						, aboveBorder  
						, lowerBorder  
						, nominalBorder
						, status[cnt]);
				}
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
				double t = 999999;
				double bit = 0;
				static const int Status = TL::IndexOf<ColorTable::items_list, Clr<BrakStrobe2<Thickness>>>::value;
				char st = StatusId<Clr<Undefined>>();
				if(s[i].hdr.G1Tof)
				{
					bit = t = 2.5e-6 * s[i].hdr.G1Tof *d.param[channel].get<gate1_TOF_WT_velocity>().value;
					if(s[i].hdr.G2Tof)
					{
						double val2 = 2.5e-6 * s[i].hdr.G2Tof * d.param[channel].get<gate2_TOF_WT_velocity>().value;
						double tt = t - val2;
						if(tt > brackStrobe)
						{
							st = Status;
							t = val2;
						}
					}
				}

				if(999999 != t) 
				{
					ret = f.Add(t, bit, st, (void *)&s[i]);
					t = f.buf[ret];

					if(cnt >= 0)
					{
						data[cnt] = t;
						scan[cnt] = (USPC7100_ASCANDATAHEADER *)f.data[ret];
						status[cnt] = f.status[ret];
						if(Status == status[cnt]) data[cnt] = f.bit2[ret];
					}
				}
				else
				{
					t = 0;
					if(cnt >= 0)
					{					
						data[cnt] = t;
						scan[cnt] = &s[i];
						status[cnt] = StatusId<Clr<Undefined>>();
					}
				}

				if(Status != status[cnt] && cnt >= 0)
				{
					StatusZoneThickness(offs, t, zone
						, aboveBorder  
						, lowerBorder  
						, nominalBorder
						, status[cnt]);
				}

				if(++cnt >= (int)dimention_of(data)) break;
			}
		}
	}
	count = cnt;
}
