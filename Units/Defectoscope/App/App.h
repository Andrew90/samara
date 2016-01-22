#pragma once

class MainWindow;

class App
{
public:
	static const int zonesCount = 60;
	static const int maxSensorCrossCount = 8;
	static const int adcBufferLength = 10000 * 60;
	static const int zone_length = 200;
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
};

extern App app;

template<class T>struct Singleton
{
	static T& Instance(){static T x; return x;}
};

