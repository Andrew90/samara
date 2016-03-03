#pragma once
#include "App.h"

void StatusZoneDefect(int offs, double data, int zone, double (&brakThreshold)[App::zonesCount], double (&klass2Threshold)[App::zonesCount], char &status);

class Compute
{
public:
	Compute();
	void Recalculation();
};

extern Compute compute;