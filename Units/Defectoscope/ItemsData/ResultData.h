#pragma once
#include "App.h"

class ResultViewerData
{
public:
    int currentOffsetZones;
	char commonStatus[App::count_zones];
	ResultViewerData();
};