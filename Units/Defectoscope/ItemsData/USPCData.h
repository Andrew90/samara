#pragma once
#include "App.h"
#include "AppBase.h"
#include "uspc7100_exports.h"


///	’ранит данные с платы USPC
class USPCData
{
public:
	int currentOffsetFrames;   ///<  Ќомер последнего кадра 
	int currentOffsetZones;	   ///< номер смещени€ кадра в зоне
	USPC7100_ASCANDATAHEADER ascanBuffer[App::count_frames];	///<собранные кадры
	int offsets[App::count_zones];  ///< смещение кадров по зонам
	int offsSensor[App::count_sensors];
	char commonStatus[App::count_zones];					///< общий статус по зонам
	double samplesPerZone;
	void Start();///< ¬ыполнить перед началом цикла сбора кадров с платы
	UCHAR *CurrentFrame(); ///<смещение в массиве buffer записи новых кадров
	void OffsetCounter(int offs);
	void SamplesPerZone(int tubeLength);
};

class USPCViewerData: public USPCData
{
public:
	double buffer[App::count_sensors][App::count_zones];	///<¬ычисленные данные разбитые по датчикам и зонам
	char status[App::count_sensors][App::count_zones];	///< статус данных по датчикам и зонам	
};

class USPCViewerThicknessData: public USPCData
{
public:
	double bufferMin[App::count_zones];	///<¬ычисленные данные разбитые по датчикам и зонам
	double bufferMax[App::count_zones];	///<¬ычисленные данные разбитые по датчикам и зонам
	char statusMin[App::count_zones];
	char statusMax[App::count_zones];	
	double scope_velocity[App::count_sensors];
};

template<class T>struct ItemData;
template<>struct ItemData<Cross>: USPCViewerData{}; 
template<>struct ItemData<Long>: USPCViewerData{};

template<>struct ItemData<Thickness>: USPCViewerThicknessData{};

/// тестирование и эмул€ци€
//struct TestUSPC
//{
//	void Init(USPCData &);
//	void InitThickness(USPCData &);
//};