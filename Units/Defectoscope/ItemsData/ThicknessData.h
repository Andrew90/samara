#pragma once
#include "App.h"
class ThicknessViewerData
{
public:
	ThicknessViewerData();	
	unsigned currentOffset;
	double zonesMin[App::zonesCount];
	double zonesMax[App::zonesCount];
	char   status[App::zonesCount];
	void Clear();
};

