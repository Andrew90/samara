#include "stdafx.h"
#include "USPCData.h"
#include "ConstData.h"
#include "AppBase.h"
#include <math.h>
#include <Mmsystem.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include "DebugMess.h"

#pragma comment(lib, "Winmm.lib")

//unsigned USPCData::previousTime;

void USPCData::Start()
{
	currentOffsetFrames = 0;	
}

///**
//	* \brief ������ �������� �� ����� ������� ������ � ����� USPC
//	* \param currentTime - ������� ����� (timeGetTime())
//	* \param strobeBit - ��� � ����� 1730 (����� ����)
//	* \param numberSavedFrames - ���������� ��������� ������ � ����� USPC
//	* \details ����� ������ ������� AddFrames ��������� 	(previousTime = currentTime)
//	* ����������  previousTime ��������� ������� ����� � �����
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

void USPCData::SamplesPerZone(int tubeLength)
{
	samplesPerZone = (double)App::zone_length * currentOffsetFrames / tubeLength;
	ZeroMemory(offsets, sizeof(offsets));
	for(int i = 0; i < App::count_zones; ++i)
	{
	   offsets[i] = int(samplesPerZone * i);
	}
	//�������� � ������� �������� �� �������
	TL::foreach<OffsetsTable::items_list, __sensors_offset_in_samples__>()(&Singleton<OffsetsTable>::Instance().items, this);
	currentOffsetZones = int((double)(tubeLength) / App::zone_length);
	for(int i = 0; i < App::count_zones; ++i)
	{
		offsets[i] /= App::count_sensors;
		offsets[i] *= App::count_sensors;
	}
}
//-----------------------------------------------------------------------------------------

// void TestUSPC::Init(USPCData &d)
// {
//	 ///�������� ��������� ������
//	 int xx[8] = {};
//	 USPC7100_ASCANDATAHEADER *b = d.ascanBuffer;
//	 int i = 0;
//	 for(; i < App::count_frames; ++i)
//	 {
//		 int c = i % App::count_sensors;
//		 b[i].Channel = c;
//		 ++c;
//		 b[i].hdr.G1Amp = xx[c - 1] / 2000 * (1 +c)/c;//10 * c + (rand() & 0xF);
//		 ++xx[c- 1];
//		 b[i].hdr.G2Amp = 8 * c + (rand() & 0xF);
//		 b[i].hdr.G1Tof = 100 * c;
//		 b[i].hdr.G2Tof = 200 * c;
//	 }
//	  ///�������� ��������  ������ �� �����
//	 for(int j = 0; j < 201; ++j)
//	 {
//		 d.offsets[j] = 800 * j;
//	 }
//
//	 d.currentOffsetFrames = 800 * 201;
//	 d.currentOffsetZones = 200;
// }

 //void TestUSPC::InitThickness(USPCData &d)
 //{
//	 ///�������� ��������� ������
//	 int xx[8] = {};
//	 USPC7100_ASCANDATAHEADER *b = d.ascanBuffer;
//	 int i = 0;
//	 double dw = M_PI / App::count_frames;
//	 for(; i < App::count_frames; ++i)
//	 {
//		 int c = i % App::count_sensors;
//		 b[i].Channel = c;
//		 ++c;
//		 b[i].hdr.G1Amp = (DWORD)(12 + 4.5 * sin(dw * i * c));
//		 ++xx[c - 1];
//		 b[i].hdr.G2Amp = 8 * c + (rand() & 0xF);
//		 b[i].hdr.G1Tof = 100 * c;
//		 b[i].hdr.G2Tof = 200 * c;
//	 }
//	  ///�������� ��������  ������ �� �����
//	 for(int j = 0; j < 201; ++j)
//	 {
//		 d.offsets[j] = 800 * j;
//	 }
 //
//	 d.currentOffsetFrames = 800 * 201;
//	 d.currentOffsetZones = 200;
 //}