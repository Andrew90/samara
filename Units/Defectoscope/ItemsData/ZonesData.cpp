#include "stdafx.h"
#include <math.h>
#include "ZonesData.h"
#include "AppBase.h"
//---------------------------------------------------------------------------------------------------------
ZonesData::ZonesData()
{}
//-------------------------------------------------------------------------------------------------------
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
void ZonesData::Clear()
{
	countZones = 0;
	ZeroMemory(zones, sizeof(zones));
	ZeroMemory(status, sizeof(status));
}
#endif
//----------------------------------------------------------------------------------------------------------

