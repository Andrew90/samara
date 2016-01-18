#pragma once
#include "App.h"
class ThicknessViewerData
{
public:
	ThicknessViewerData();	
	int currentOffset;
	double zonesMin[App::zonesCount];
	double zonesMax[App::zonesCount];
	char   status[App::zonesCount];
	void Clear();
};

