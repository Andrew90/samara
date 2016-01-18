#include "stdafx.h"
#include "LongData.h"
#include "ConstData.h"
#include "AppBase.h"

void LongData::Start()
{
	currentOffset = 0;
}

LongViewerData::LongViewerData()
	: currentOffset(0)
{
	//test
	double t = 0;
	currentOffset = 33;
	for(int sens = 0; sens < App::maxSensorCrossCount; ++sens)
	{
		for(unsigned zone = 1; zone < currentOffset - 1; ++zone)
		{
			t += 0.11;
			if(t > 3) t = 0;
			buffer[sens][zone] = t;
			if(t < 1) status[sens][zone] = Stat<Nominal>::value;
			else if(t < 3) status[sens][zone] =  Stat<Treshold2Class>::value;
			else  status[sens][zone] = Stat<Defect>::value;
		}
	}
	for(unsigned sens = 0; sens < App::maxSensorCrossCount; ++sens)
	{
		status[sens][0] = Stat<DeathZone>::value;
		status[sens][currentOffset - 1] = Stat<DeathZone>::value;
	}
	//test
}