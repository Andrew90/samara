#include "stdafx.h"
#include "USPCData.h"
#include "ConstData.h"
#include "AppBase.h"
#include <math.h>
#include <Mmsystem.h>
#define _USE_MATH_DEFINES
#include <cmath>

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
//			if(App::zonesCount < currentOffsetZones) ++currentOffsetZones;
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
//		if(App::zonesCount < currentOffsetZones) ++currentOffsetZones;
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
			double t = (double)p->samplesPerZone * o->value / App::zone_length / App::count_sensors;
			p->offsSensor[i] = int(t) * App::count_sensors;
		}
	};
}

void USPCData::SamplesPerZone(int tubeLength)
{
	samplesPerZone = (double)App::zone_length * currentOffsetFrames 
		/ (tubeLength + Singleton<OffsetsTable>::Instance().items.get<Offset<7>>().value);
	ZeroMemory(offsets, sizeof(offsets));
	for(int i = 1; i < App::zonesCount; ++i)
	{
	   offsets[i] = int(samplesPerZone * i);
	}
	//смещение в отчётах датчиков на каретке
	TL::foreach<OffsetsTable::items_list, __sensors_offset_in_samples__>()(&Singleton<OffsetsTable>::Instance().items, this);
}
//-----------------------------------------------------------------------------------------

 void TestUSPC::Init(USPCData &d)
 {
	 ///эмитация считанных данных
	 int xx[8] = {};
	 USPC7100_ASCANDATAHEADER *b = d.ascanBuffer;
	 int i = 0;
	 for(; i < App::count_frames; ++i)
	 {
		 int c = i % App::count_sensors;
		 b[i].Channel = c;
		 ++c;
		 b[i].hdr.G1Amp = xx[c - 1] / 2000 * (1 +c)/c;//10 * c + (rand() & 0xF);
		 ++xx[c- 1];
		 b[i].hdr.G2Amp = 8 * c + (rand() & 0xF);
		 b[i].hdr.G1Tof = 100 * c;
		 b[i].hdr.G2Tof = 200 * c;
	 }
	  ///эмитация смещения  кадров по зонам
	 for(int j = 0; j < 51; ++j)
	 {
		 d.offsets[j] = 16000 * j;
	 }

	 d.currentOffsetFrames = 16000 * 51;
	 d.currentOffsetZones = 50;
 }

 void TestUSPC::InitThickness(USPCData &d)
 {
	 ///эмитация считанных данных
	 int xx[8] = {};
	 USPC7100_ASCANDATAHEADER *b = d.ascanBuffer;
	 int i = 0;
	 double dw = M_PI / App::count_frames;
	 for(; i < App::count_frames; ++i)
	 {
		 int c = i % App::count_sensors;
		 b[i].Channel = c;
		 ++c;
		 b[i].hdr.G1Amp = (DWORD)(12 + 4.5 * sin(dw * i * c));
		 ++xx[c - 1];
		 b[i].hdr.G2Amp = 8 * c + (rand() & 0xF);
		 b[i].hdr.G1Tof = 100 * c;
		 b[i].hdr.G2Tof = 200 * c;
	 }
	  ///эмитация смещения  кадров по зонам
	 for(int j = 0; j < 51; ++j)
	 {
		 d.offsets[j] = 16000 * j;
	 }

	 d.currentOffsetFrames = 16000 * 51;
	 d.currentOffsetZones = 50;
 }