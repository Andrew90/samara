#include "stdafx.h"
#include "USPCData.h"
#include "ConstData.h"
#include "AppBase.h"
#include <math.h>
#include <Mmsystem.h>

#pragma comment(lib, "Winmm.lib")

unsigned USPCData::previousTime;

void USPCData::Start()
{
	currentOffsetFrames = 0;
	currentOffsetZones = 0;
	previousStrobeBit = false;
}

/**
	* \brief ������ �������� �� ����� ������� ������ � ����� USPC
	* \param currentTime - ������� ����� (timeGetTime())
	* \param strobeBit - ��� � ����� 1730 (����� ����)
	* \param numberSavedFrames - ���������� ��������� ������ � ����� USPC
	* \details ����� ������ ������� AddFrames ��������� 	(previousTime = currentTime)
	* ����������  previousTime ��������� ������� ����� � �����
	*/

void USPCData::AddFrames(unsigned currentTime, bool strobeBit, unsigned numberSavedFrames)
{
	if(strobeBit)
	{
		if(numberSavedFrames)
		{
			offsets[currentOffsetZones] = currentOffsetFrames + numberSavedFrames;
			if(App::zonesCount < currentOffsetZones) ++currentOffsetZones;
		}
		else
		{
			previousStrobeBit = true;
			strobeTime = currentTime; 
		}
	}
	else if(previousStrobeBit)
	{
		double dt = currentTime - previousTime;
		double dx = strobeTime -  previousTime;
		offsets[currentOffsetZones] = currentOffsetFrames + int(dx * numberSavedFrames / dt);
		if(App::zonesCount < currentOffsetZones) ++currentOffsetZones;
		previousStrobeBit = false;
	}

	currentOffsetFrames += numberSavedFrames;
}

unsigned char *USPCData::CurrentFrame()
{
	return (BYTE *)&ascanBuffer[currentOffsetFrames];
}
//-----------------------------------------------------------------------------------------

 void TestUSPC::Init(USPCData &d)
 {
	 ///�������� ��������� ������
	 int xx[8] = {};
	 USPC7100_ASCANDATAHEADER *b = d.ascanBuffer;
	 int i = 0;
	 for(; i < App::count_frames; ++i)
	 {
		 int c = i % App::count_sensors;
		 b[i].Channel = c;
		 ++c;
		 b[i].hdr.G1Amp = xx[c - 1] / 2000 * c;//10 * c + (rand() & 0xF);
		 ++xx[c- 1];
		 b[i].hdr.G2Amp = 8 * c + (rand() & 0xF);
		 b[i].hdr.G1Tof = 100 * c;
		 b[i].hdr.G2Tof = 200 * c;
		 //BYTE *p = b[i].Point;
		 //double dw = 2 * 3.1415 / 512;
		 //for(int j = 0; j < 512; ++j)
		 //{
		//	 p[j] = BYTE(127 * (sin(dw * j * c + 0.1 * dw * rand() / RAND_MAX) + 1));
		 //}
	 }
	 //for(; i < App::count_frames; ++i)
	 //{
	//	 memcpy(&b[i], &b[i % App::count_sensors], sizeof(USPC7100_ASCANDATAHEADER));
	 //}
	  ///�������� ��������  ������ �� �����
	 for(int j = 0; j < 51; ++j)
	 {
		 d.offsets[j] = 16000 * j;
	 }

	 d.currentOffsetFrames = 16000 * 51;
	 d.currentOffsetZones = 50;
 }