#pragma once
#include "App.h"

class USPCData
{
public:
	int currentOffset;
	USPC7100_ASCANDATAHEADER buffer[App::count_frames];
	void Start();
	void AddFrames(unsigned);
	unsigned char *CurrentFrame();
};

class USPCViewerData
{
public:
    int currentOffset;
	double buffer[App::count_sensors][App::zonesCount];
	char status[App::count_sensors][App::zonesCount];
	char commonStatus[App::count_sensors][App::zonesCount];
	int offsets[App::zonesCount + 1];
	USPCViewerData();
};

class USPCViewerThicknessData
{
public:
    int currentOffset;
	double buffer[App::count_sensors][App::zonesCount];
	char status[App::count_sensors][App::zonesCount];
	double zonesMin[App::zonesCount];
	double zonesMax[App::zonesCount];
	char commonStatus[App::zonesCount];
	int offsets[App::zonesCount + 1];
	USPCViewerThicknessData();
};

struct Long{};
struct Cross{};
struct Thickness{};

template<class T>struct ItemData: USPCViewerData{}; 
template<>struct ItemData<Thickness>: USPCViewerThicknessData{};