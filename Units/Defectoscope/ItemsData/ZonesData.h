#pragma once
class ZonesData
{
public:
	static const int zonesCount = 2048;
	int countZones;
//	double &thresholdClass1;
//	double &thresholdMin;
//	double &thresholdMax;
	double zones[zonesCount];
	char status[zonesCount];
	ZonesData();
	void Clear();
	bool SetZones(int countZones, double *data);
};

//extern ZonesData zonesData;
