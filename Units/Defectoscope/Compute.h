#pragma once
#include "App.h"

template<class T>void StatusZoneDefect(int offs, double data, int zone, double (&brakThreshold)[App::zonesCount], double (&klass2Threshold)[App::zonesCount], char &status)
{
	 if(data > brakThreshold[zone])
	 {
		 status = StatusId<Clr<BorderDefect<T>>>();
	 }
	 else  if(data > klass2Threshold[zone])
	 {
		 status = StatusId<Clr<BorderKlass2<T>>>();
	 }
	 else
	 {
		status = StatusId<Clr<Nominal>>();
	 }
}


void StatusZoneThickness(int offs, double data, int zone, double (&maxThreshold)[App::zonesCount]
    , double (&minThreshold)[App::zonesCount], double (&nominalTreshold)[App::zonesCount], char &status);

class Compute
{
public:
	Compute();
	void Recalculation();
	//void Test();
};

extern Compute compute;