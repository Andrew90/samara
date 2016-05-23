#pragma once
#include "App.h"
#include "AppBase.h"


///	������ ������ � ����� USPC
class USPCData
{
public:
	int currentOffsetFrames;   ///<  ����� ���������� ����� 
	int currentOffsetZones;	   ///< ����� �������� ����� � ����
	USPC7100_ASCANDATAHEADER ascanBuffer[App::count_frames];	///<��������� �����
	int offsets[App::zonesCount];  ///< �������� ������ �� �����
	int offsSensor[App::count_sensors];
	char commonStatus[App::zonesCount];					///< ����� ������ �� �����
	double samplesPerZone;
	void Start();///< ��������� ����� ������� ����� ����� ������ � �����
	UCHAR *CurrentFrame(); ///<�������� � ������� buffer ������ ����� ������
	void OffsetCounter(int offs);
	void SamplesPerZone(int tubeLength);
};

class USPCViewerData: public USPCData
{
public:
	double buffer[App::count_sensors][App::zonesCount];	///<����������� ������ �������� �� �������� � �����
	char status[App::count_sensors][App::zonesCount];	///< ������ ������ �� �������� � �����	
};

class USPCViewerThicknessData: public USPCData
{
public:
	double bufferMin[App::zonesCount];	///<����������� ������ �������� �� �������� � �����
	double bufferMax[App::zonesCount];	///<����������� ������ �������� �� �������� � �����
	char statusMin[App::zonesCount];
	char statusMax[App::zonesCount];	
};

template<class T>struct ItemData: USPCViewerData{}; 

template<>struct ItemData<Thickness>: USPCViewerThicknessData{};

/// ������������ � ��������
struct TestUSPC
{
	void Init(USPCData &);
	void InitThickness(USPCData &);
};