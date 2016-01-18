#include "stdafx.h"
#include <math.h>
#include "ThicknessData.h"
#include "AppBase.h"
//#include "PrimaryData.h"
//---------------------------------------------------------------------------------------------------------
ThicknessViewerData::ThicknessViewerData()
	: currentOffset(0)
{
	
	currentOffset = 30;
	double dx = 2 * 3.14 / App::zonesCount * 5;
	for(int i = 0; i < currentOffset; ++ i)
	{
		zonesMin[i] = sin(dx * i) * 5 + 8;
		zonesMax[i] = zonesMin[i] + 2;
		status[i] = i % 10 + 1;
	}

}
//-------------------------------------------------------------------------------------------------------
/*
#if 0
void ThicknessData::Clear()
{
	tresholdClass1  = -1;
	tresholdClass2  = -1;
	countZones = 0;
	for(int i = 0; i < App::zonesCount; ++ i)
	{
		for(int j = 0; j < COUNT_SENSORS; ++j) zones[j][i] = -1;
	}
}
#else
#pragma message("тестовый ThicknessData::Clear()")
void ThicknessData::Clear()
{
	countZones = 0;
	zones[0] = 0;
}
#endif
*/
//----------------------------------------------------------------------------------------------------------
void ThicknessViewerData::Clear()
{
	memset(status, 0, sizeof(status));
}
//ThicknessData thicknessData;
//ThicknessData sensorsData[count_sensors];
