#pragma once
#include "App.h"
#include "AppBase.h"


///	’ранит данные с платы USPC
class USPCData
{
public:
	int currentOffsetFrames;   ///<  Ќомер последнего кадра 
	int currentOffsetZones;	   ///< номер смещени€ кадра в зоне
	USPC7100_ASCANDATAHEADER ascanBuffer[App::count_frames];	///<собранные кадры
	int offsets[App::zonesCount];  ///< смещение кадров по зонам
	int offsSensor[App::count_sensors];
	char commonStatus[App::zonesCount];					///< общий статус по зонам
	double samplesPerZone;
	void Start();///< ¬ыполнить перед началом цикла сбора кадров с платы
	UCHAR *CurrentFrame(); ///<смещение в массиве buffer записи новых кадров
	void OffsetCounter(int offs);
	void SamplesPerZone(int tubeLength);
};

class USPCViewerData: public USPCData
{
public:
	double buffer[App::count_sensors][App::zonesCount];	///<¬ычисленные данные разбитые по датчикам и зонам
	char status[App::count_sensors][App::zonesCount];	///< статус данных по датчикам и зонам	
};

class USPCViewerThicknessData: public USPCData
{
public:
	double bufferMin[App::zonesCount];	///<¬ычисленные данные разбитые по датчикам и зонам
	double bufferMax[App::zonesCount];	///<¬ычисленные данные разбитые по датчикам и зонам
	char statusMin[App::zonesCount];
	char statusMax[App::zonesCount];	
};

template<class T>struct ItemData: USPCViewerData{}; 

template<>struct ItemData<Thickness>: USPCViewerThicknessData{};

/// тестирование и эмул€ци€
struct TestUSPC
{
	void Init(USPCData &);
	void InitThickness(USPCData &);
};