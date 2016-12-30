#include "stdafx.h"
#include "ItemsData/USPCData.h"
#include "ItemsData/ConstData.h"
#include "App/AppBase.h"
#include <math.h>
#include <Mmsystem.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include "debug_tools/DebugMess.h"
#include "USPC/ut_files.h"
#include "Ini\ItemIni.h"

#pragma comment(lib, "Winmm.lib")

//unsigned USPCData::previousTime;

void USPCData::Start()
{
	currentOffsetFrames = 0;	
}

///**
//	* \brief Расчёт разбития по зонам входных данных с платы USPC
//	* \param currentTime - текущее время (timeGetTime())
//	* \param strobeBit - бит с платы 1730 (строб зоны)
//	* \param numberSavedFrames - количество считанных данных с плату USPC
//	* \details После вызова функции AddFrames выполнить 	(previousTime = currentTime)
//	* парраметру  previousTime присвоить текущее время в тиках
//	*/
//
//void USPCData::AddFrames(unsigned currentTime, bool strobeBit, unsigned numberSavedFrames)
//{
//	if(strobeBit)
//	{
//		if(numberSavedFrames)
//		{
//			offsets[currentOffsetZones] = currentOffsetFrames + numberSavedFrames;
//			if(App::count_zones < currentOffsetZones) ++currentOffsetZones;
//		}
//		else
//		{
//			previousStrobeBit = true;
//			strobeTime = currentTime; 
//		}
//	}
//	else if(previousStrobeBit)
//	{
//		double dt = currentTime - previousTime;
//		double dx = strobeTime -  previousTime;
//		offsets[currentOffsetZones] = currentOffsetFrames + int(dx * numberSavedFrames / dt);
//		if(App::count_zones < currentOffsetZones) ++currentOffsetZones;
//		previousStrobeBit = false;
//	}
//
//	currentOffsetFrames += numberSavedFrames;
//}

UCHAR *USPCData::CurrentFrame()
{
	return (UCHAR *)&ascanBuffer[currentOffsetFrames];
}
void USPCData::OffsetCounter(int offs)
{
	currentOffsetFrames += offs;
}

namespace
{
	template<class O, class P>struct __sensors_offset_in_samples__
	{
		void operator()(O *o, P *p)
		{
			static const int i = TL::IndexOf<OffsetsTable::items_list, O>::value;
			double t = (double)p->samplesPerZone * o->value / App::zone_length;// / App::count_sensors;
			p->offsSensor[i] = (int)t;
			p->offsSensor[i] /= App::count_sensors;
			p->offsSensor[i] *= App::count_sensors;
		}
	};
}

void USPCData::SamplesPerZone(int tubeLength, int deadArea0, int deadArea1)
{
	samplesPerZone = (double)App::zone_length * currentOffsetFrames 
		/ (tubeLength + App::lengthCaretka);
	ZeroMemory(offsets, sizeof(offsets));
	for(int i = 0; i < App::count_zones; ++i)
	{
	   offsets[i] = int(samplesPerZone * i);
	}
	//смещение в отчётах датчиков на каретке
	TL::foreach<OffsetsTable::items_list, __sensors_offset_in_samples__>()(&Singleton<OffsetsTable>::Instance().items, this);
	currentOffsetZones = int((double)(tubeLength) / App::zone_length);
	int lastZoneSize = tubeLength - currentOffsetZones * App::zone_length;
	if(lastZoneSize > App::zone_length / 3)  ++currentOffsetZones;
	//число отчётов в мёртвой зоне начало
	double t = deadArea0;//Singleton<DeadAreaTable>::Instance().items.get<DeadAreaMM0>().value;
	t *= samplesPerZone;
	t /=  App::zone_length;
    deadZoneSamplesBeg  = (int)t;
	deadZoneSamplesBeg /= App::count_sensors;
	deadZoneSamplesBeg *= App::count_sensors;
	//число отчётов в мёртвой зоне конец
	t = tubeLength - deadArea1;//Singleton<DeadAreaTable>::Instance().items.get<DeadAreaMM1>().value;
	t *= samplesPerZone;
	t /=  App::zone_length;
    deadZoneSamplesEnd  = (int)t;

	deadZoneSamplesEnd /= App::count_sensors;
	--deadZoneSamplesEnd;
	deadZoneSamplesEnd *= App::count_sensors;

	for(int i = 0; i < App::count_zones; ++i)
	{
		offsets[i] /= App::count_sensors;
		offsets[i] *= App::count_sensors;
	}
}
//-----------------------------------------------------------------------------------------
//void USPCIniFile::Init()
//{
//	wchar_t path[256];
//	wchar_t section[256];
//	if(ExistCurrentUSPCFile(path))
//	{
//		for(int i = 0; i < dimention_of(scope_range); ++i)
//		{
//		   wsprintf(section, L"Test %d", i);
//		   scope_range    [i]   = ItemIni::Get(section, L"scope_range", 12, path); 
//		   scope_offset   [i]   = ItemIni::Get(section, L"scope_offset", 40, path); 
//		   gateIF_position[i]   = ItemIni::Get(section, L"gateIF_position", 30.769, path);
//		   gateIF_width   [i]   = ItemIni::Get(section, L"gateIF_width", 2, path); 
//		   gateIF_level   [i]   = ItemIni::Get(section, L"gateIF_level", 20, path); 
//		   gate1_width    [i]   = ItemIni::Get(section, L"gate1_width", 7.311, path); 
//		   gate1_position [i]   = ItemIni::Get(section, L"gate1_position", 1.231, path);
//		   gate1_level    [i]   = ItemIni::Get(section, L"gate1_level", 30, path);
//		}
//	}
//}