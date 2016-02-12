#include "stdafx.h"
#include "USPCData.h"
#include "ConstData.h"
#include "AppBase.h"

void USPCData::Start()
{
	currentOffset = 0;
}

void USPCData::AddFrames(unsigned count)
{
	currentOffset += count;
}

unsigned char *USPCData::CurrentFrame()
{
	return (unsigned char *)&buffer[currentOffset];
}

USPCViewerData::USPCViewerData()
	: currentOffset(0)
{
	//test
	double t = 0;
	currentOffset = 33;
	for(int sens = 0; sens < App::count_sensors; ++sens)
	{
		for(int zone = 1; zone < currentOffset - 1; ++zone)
		{
			t += 0.1;
			if(t > 3) t = 0;
			buffer[sens][zone] = t;
			if(t < 1) status[sens][zone] = Stat<Nominal>::value;
			else if(t < 3) status[sens][zone] =  Stat<Treshold2Class>::value;
			else  status[sens][zone] = Stat<Defect>::value;
		}
	}
	for(unsigned sens = 0; sens < App::count_sensors; ++sens)
	{
		status[sens][0] = Stat<DeathZone>::value;
		status[sens][currentOffset - 1] = Stat<DeathZone>::value;
	}
	//test
}

USPCViewerThicknessData::USPCViewerThicknessData()
	: currentOffset(0)
{
	
}