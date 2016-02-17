#pragma once
#include "uspc7100_exports.h"
class MainWindow;

struct Long{};
struct Cross{};
struct Thickness{};

class App
{
public:
	static const int zonesCount = 60;
	static const int count_sensors = 8;
	static const int zone_length = 200;
	static const int count_frames = 512 * 1024 * 1024 / sizeof(USPC7100_ASCANDATAHEADER);
	static const int count_frames_per_channel = count_frames / count_sensors;
	static HANDLE ProgrammExitEvent;
	static HANDLE ProgrammContinueEvent;
	static HANDLE ProgrammStopEvent;
	static bool measurementOfRunning;
public:
	MainWindow &mainWindow;
public:
	App();
	void Init();
	void Destroy();
	void MainWindowTopLabel(wchar_t *);
	void MainWindowBottomLabel(int, wchar_t *);  
	void MainWindowUpdate();
};

extern App app;

template<class T>struct Singleton
{
	static T& Instance(){static T x; return x;}
};



