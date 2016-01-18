#pragma once
#include "App.h"

class ResultViewerData
{
public:
    int currentOffset;
	int status[App::zonesCount];
	ResultViewerData();
};