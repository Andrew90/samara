#pragma once
#include "App.h"

///	Хранит данные с платы USPC
class USPCData
{
public:
	int currentOffsetFrames;   ///<  Номер последнего кадра 
	int currentOffsetZones;	   ///< номер смещения кадра в зоне
	unsigned strobeTime;	   ///<время последнего строба
	static unsigned previousTime; ///<время предыдущего сбора кадров
	bool previousStrobeBit;			///<был ли в предыдущем сборе поднят бит зоны
	USPC7100_ASCANDATAHEADER buffer[App::count_frames];	///<собранные кадры
	int offsets[App::zonesCount + 1];  ///< смещение кадров по зонам
	void Start();///< Выполнить перед началом цикла сбора кадров с платы
	void AddFrames(unsigned currentTime, bool strobeBit, unsigned numberSavedFrames); ///<добавляет считанные кадры с платы в массив
	BYTE *CurrentFrame(); ///<смещение в массиве buffer записи новых кадров
};

class USPCViewerData
{
public:
	USPCData &uspcData;
	double buffer[App::count_sensors][App::zonesCount];	///<Вычисленные данные разбитые по датчикам изонам
	char status[App::count_sensors][App::zonesCount];	///< статус данных по датчикам изонам
	char commonStatus[App::zonesCount];					///< общий статус по зонам
	USPCViewerData();									
};

class USPCViewerThicknessData: public USPCViewerData
{
public:
	double zonesMin[App::zonesCount];	///<минимальная толщина в зоне
	double zonesMax[App::zonesCount];	///<максимальная толщина в зоне
	USPCViewerThicknessData();
};

struct Long{};
struct Cross{};
struct Thickness{};

template<class T>struct ItemData: USPCViewerData{}; 
template<>struct ItemData<Thickness>: USPCViewerThicknessData{};

/// тестирование и эмуляция
struct TestUSPC
{
	void Init(USPCData &);
};