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
	int deadZoneSamplesBeg, deadZoneSamplesEnd;
	void Start();///< ¬ыполнить перед началом цикла сбора кадров с платы
	UCHAR *CurrentFrame(); ///<смещение в массиве buffer записи новых кадров
	void OffsetCounter(int offs);
	void SamplesPerZone(int tubeLength, int deadArea0, int deadArea1);
};

#define USPC_PARAM(name, val)struct name\
{\
	double value;\
	name(): value(val){}\
	char *Name(){return #name;}\
};

USPC_PARAM(scope_range    , 12.01)
USPC_PARAM(scope_offset   , 40.01)
USPC_PARAM(gateIF_position, 41.01)
USPC_PARAM(gateIF_width   , 2.01)
USPC_PARAM(gateIF_level   , 20.01)
USPC_PARAM(gate1_width    , 7.31)
USPC_PARAM(gate1_position , 1.21)
USPC_PARAM(gate1_level 	  , 30.01)
USPC_PARAM(gate1_TOF_WT_velocity , 6400.01)

#undef USPC_PARAM

class USPCViewerData: public USPCData
{
public:
	double buffer[App::count_sensors][App::count_zones];	///<¬ычисленные данные разбитые по датчикам и зонам
	char status[App::count_sensors][App::count_zones];	///< статус данных по датчикам и зонам	
	typedef TL::MkTlst<
		scope_range    
		, scope_offset   
	//	, gateIF_position
	//	, gateIF_width   
	//	, gateIF_level   
		, gate1_width    
		, gate1_position 
		, gate1_level 	  
		, gate1_TOF_WT_velocity
	>::Result param_list;
	typedef TL::Factory<param_list> TParam;
	TParam param[App::count_sensors]; 
};

class USPCViewerThicknessData: public USPCData
{
public:
	double bufferMin[App::count_zones];	///<¬ычисленные данные разбитые по датчикам и зонам
	double bufferMax[App::count_zones];	///<¬ычисленные данные разбитые по датчикам и зонам
	char statusMin[App::count_zones];
	char statusMax[App::count_zones];	
	//double scope_velocity[App::count_sensors];
	typedef TL::MkTlst<
		scope_range    
		, scope_offset   
		, gateIF_position
		, gateIF_width   
		, gateIF_level   
		, gate1_width    
		, gate1_position 
		, gate1_level 	  
		, gate1_TOF_WT_velocity
	>::Result param_list;
	typedef TL::Factory<param_list> TParam;
	TParam param[App::count_sensors]; 
};

template<class T>struct ItemData;
template<>struct ItemData<Cross>: USPCViewerData{}; 
template<>struct ItemData<Long>: USPCViewerData{};
template<>struct ItemData<Thickness>: USPCViewerThicknessData{};


//class USPCIniFile
//{
//public:
//   double scope_range    [8];
//   double scope_offset   [8];
//   double gateIF_position[8];
//   double gateIF_width   [8];
//   double gateIF_level   [8];
//   double gate1_width    [8];
//   double gate1_position [8];
//   double gate1_level    [8];
//   void Init();
//};