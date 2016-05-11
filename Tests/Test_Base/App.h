#pragma once
#include "uspc7100_exports.h"

struct Long{};
struct Cross{};
struct Thickness{};

class App
{
public:
	static const int zonesCount = 4 * 60;
	static const int count_sensors = 8;
	static const int zone_length = 50;
	static const int count_frames = 100000;
};

template<class T>struct Singleton
{
	static T& Instance(){static T x; return x;}
};