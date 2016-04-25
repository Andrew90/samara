#pragma once
#include "App.h"
#include "AppBase.h"

///	’ранит данные с платы USPC
class USPCData
{
public:
	int currentOffsetFrames;   ///<  Ќомер последнего кадра 
	int currentOffsetZones;	   ///< номер смещени€ кадра в зоне
	unsigned strobeTime;	   ///<врем€ последнего строба
	static unsigned previousTime; ///<врем€ предыдущего сбора кадров
	bool previousStrobeBit;			///<был ли в предыдущем сборе подн€т бит зоны
	USPC7100_ASCANDATAHEADER ascanBuffer[App::count_frames];	///<собранные кадры
	int offsets[App::zonesCount + 2];  ///< смещение кадров по зонам
	char commonStatus[App::zonesCount];					///< общий статус по зонам
	void Start();///< ¬ыполнить перед началом цикла сбора кадров с платы
	void AddFrames(unsigned currentTime, bool strobeBit, unsigned numberSavedFrames); ///<добавл€ет считанные кадры с платы в массив
	BYTE *CurrentFrame(); ///<смещение в массиве buffer записи новых кадров
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