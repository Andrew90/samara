#pragma once
#include "App.h"

class CrossData
{
public:
	unsigned currentOffset;
	double buffer[App::maxSensorCrossCount][App::adcBufferLength];
	void Start();
};

class CrossViewerData
{
public:
    unsigned currentOffset;
	double buffer[App::maxSensorCrossCount][App::zonesCount];
	int status[App::maxSensorCrossCount][App::zonesCount];
	int offsets[App::zonesCount + 1];
	CrossViewerData();
};