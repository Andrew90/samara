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
	void Start();///< ��������� ����� ������� ����� ����� ������ � �����
	UCHAR *CurrentFrame(); ///<�������� � ������� buffer ������ ����� ������
	void OffsetCounter(int offs);
	void SamplesPerZone(int tubeLength);
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

template<>struct ItemData<Thickness>: USPCViewerThicknessData{};

/// ������������ � ��������
//struct TestUSPC
//{
//	void Init(USPCData &);
//	void InitThickness(USPCData &);
//};