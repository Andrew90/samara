#pragma once
#include "App.h"

class ResultViewerData
{
public:
    int currentOffset;
	char commonStatus[App::count_zones];
	ResultViewerData();
};