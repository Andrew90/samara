#pragma once
class ZonesData
{
public:
	static const int zonesCount = 2048;
	int countZones;
	double zones[zonesCount];
	char status[zonesCount];
	ZonesData();
	void Clear();
	bool SetZones(int countZones, double *data);
};

//extern ZonesData zonesData;
