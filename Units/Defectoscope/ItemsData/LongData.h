#pragma once
#include "App.h"

class LongData
{
public:
	int currentOffset;
	double buffer[App::maxSensorCrossCount][App::adcBufferLength];
	void Start();
};

class LongViewerData
{
public:
    int currentOffset;
	double buffer[App::maxSensorCrossCount][App::zonesCount];
	int status[App::maxSensorCrossCount][App::zonesCount];
	int offsets[App::zonesCount + 1];
	LongViewerData();
};