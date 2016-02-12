#pragma once
#include "App.h"

///	������ ������ � ����� USPC
class USPCData
{
public:
	int currentOffsetFrames;   ///<  ����� ���������� ����� 
	int currentOffsetZones;	   ///< ����� �������� ����� � ����
	unsigned strobeTime;	   ///<����� ���������� ������
	static unsigned previousTime; ///<����� ����������� ����� ������
	bool previousStrobeBit;			///<��� �� � ���������� ����� ������ ��� ����
	USPC7100_ASCANDATAHEADER buffer[App::count_frames];	///<��������� �����
	int offsets[App::zonesCount + 1];  ///< �������� ������ �� �����
	void Start();///< ��������� ����� ������� ����� ����� ������ � �����
	void AddFrames(unsigned currentTime, bool strobeBit, unsigned numberSavedFrames); ///<��������� ��������� ����� � ����� � ������
	BYTE *CurrentFrame(); ///<�������� � ������� buffer ������ ����� ������
};

class USPCViewerData
{
public:
	USPCData &uspcData;
	double buffer[App::count_sensors][App::zonesCount];	///<����������� ������ �������� �� �������� ������
	char status[App::count_sensors][App::zonesCount];	///< ������ ������ �� �������� ������
	char commonStatus[App::zonesCount];					///< ����� ������ �� �����
	USPCViewerData();									
};

class USPCViewerThicknessData: public USPCViewerData
{
public:
	double zonesMin[App::zonesCount];	///<����������� ������� � ����
	double zonesMax[App::zonesCount];	///<������������ ������� � ����
	USPCViewerThicknessData();
};

struct Long{};
struct Cross{};
struct Thickness{};

template<class T>struct ItemData: USPCViewerData{}; 
template<>struct ItemData<Thickness>: USPCViewerThicknessData{};

/// ������������ � ��������
struct TestUSPC
{
	void Init(USPCData &);
};