#pragma once
#include "App.h"
#include "AppBase.h"
#include "uspc7100_exports.h"


///	������ ������ � ����� USPC
class USPCData
{
public:
	int currentOffsetFrames;   ///<  ����� ���������� ����� 
	int currentOffsetZones;	   ///< ����� �������� ����� � ����
	USPC7100_ASCANDATAHEADER ascanBuffer[App::count_frames];	///<��������� �����
	int offsets[App::count_zones];  ///< �������� ������ �� �����
	int offsSensor[App::count_sensors];
	char commonStatus[App::count_zones];					///< ����� ������ �� �����
	double samplesPerZone;
	int deadZoneSamplesBeg, deadZoneSamplesEnd;
	void Start();///< ��������� ����� ������� ����� ����� ������ � �����
	UCHAR *CurrentFrame(); ///<�������� � ������� buffer ������ ����� ������
	void OffsetCounter(int offs);
	void SamplesPerZone(int tubeLength, int deadArea0, int deadArea1);
};

class USPCViewerData: public USPCData
{
public:
	double buffer[App::count_sensors][App::count_zones];	///<����������� ������ �������� �� �������� � �����
	char status[App::count_sensors][App::count_zones];	///< ������ ������ �� �������� � �����	
};

class USPCViewerThicknessData: public USPCData
{
public:
	double bufferMin[App::count_zones];	///<����������� ������ �������� �� �������� � �����
	double bufferMax[App::count_zones];	///<����������� ������ �������� �� �������� � �����
	char statusMin[App::count_zones];
	char statusMax[App::count_zones];	
	double scope_velocity[App::count_sensors];
};

template<class T>struct ItemData;
template<>struct ItemData<Cross>: USPCViewerData{}; 
template<>struct ItemData<Long>: USPCViewerData{};

template<>struct ItemData<Thickness>: USPCViewerThicknessData
{
	ItemData()
	{
		for(int i = 0; i < App::count_sensors; ++i)
		{
			scope_velocity[i] = 6400.0;
		}
	}
};

class USPCIniFile
{
public:
   double scope_range    [8];
   double scope_offset   [8];
   double gateIF_position[8];
   double gateIF_width   [8];
   double gateIF_level   [8];
   double gate1_width    [8];
   double gate1_position [8];
   double gate1_level    [8];
   void Init();
};