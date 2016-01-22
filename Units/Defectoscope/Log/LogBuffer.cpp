#include "stdafx.h"
#include <atlsimpstr.h>
#include "LogBuffer.h"
#include "DebugMess.h"
wchar_t name[] = L"Parallel hatch in log space ";

#define d_mess

namespace LogSpace{
	static const unsigned SizeBuffer = 1024;
	struct TMapData
	{
		volatile LONG head;
		volatile LONG tail;
		Log::TData data[SizeBuffer];
	};
}
LONG Log::lastMessage = -1;
struct Log::Inner
{
	HANDLE hMapFile;
	LogSpace::TMapData *map;
	bool IsRow(int row, Log::TData *&d)
	{
		if(map->head - map->tail > LogSpace::SizeBuffer) map->tail = map->head - LogSpace::SizeBuffer;
		if(map->head - map->tail > row)
		{
			int i = map->head - row;
			i %= LogSpace::SizeBuffer; 
			d = &map->data[i];
			return true;
		}
		return false;
	}
	void Get(int i, Log::TData *&d)
	{
	   d = &map->data[i];
	}
};

Log::Log(Inner &inner)
	: inner(inner)
{
	inner.hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,    // use paging file
		NULL,                    // default security 
		PAGE_READWRITE,          // read/write access
		0,                       // max. object size 
		sizeof(LogSpace::TMapData),                // buffer size  
		name);                 // name of mapping object
	
	unsigned res = GetLastError();
	inner.map = (LogSpace::TMapData *) MapViewOfFile(inner.hMapFile,   // handle to map object
		FILE_MAP_ALL_ACCESS, // read/write permission
		0,                   
		0,                   
		sizeof(LogSpace::TMapData));           

	if (inner.map == NULL) 
	{ 
		d_mess("Could not map view of file (%d).\n", 
			GetLastError()); 
		return;
	}
	if(0 == res)
	{
		inner.map->head = 0;
		inner.map->tail = 0;
	}
	else
	{
		if(inner.map->head - inner.map->tail > LogSpace::SizeBuffer) 	inner.map->tail = inner.map->head - LogSpace::SizeBuffer;
	}
}

void Log::Insert(int id, double val, void (*Do)(LONG , LONG &))
{
	LONG i = _InterlockedIncrement(&inner.map->head);
	i &= LogSpace::SizeBuffer - 1;
	(*Do)(i, lastMessage);
	inner.map->data[i].id = id;
	inner.map->data[i].time = GetTickCount();
	inner.map->data[i].value = val;
}

bool Log::IsRow(int row, TData *&d)
{
	return hidden.inner.IsRow(row, d);
}

int Log::LastMessageIndex()
{
	return hidden.inner.map->head;
}

bool Log::LastMessage(TData *&d)
{
	int t = InterlockedExchange(&lastMessage, -1);
	if(-1 == t) return false;
	hidden.inner.Get(t, d);
	return true;
}


class Initialization
{
	friend class Log;
	HANDLE hTimer;
	LogMess::FactoryMessages &f;
	void Do()
	{
		Log::TData *d;
		if(Log::LastMessage(d))
		{
			wchar_t mess[256];
			unsigned textColor, backColor;
			f.Color(d->id, backColor, textColor);
			char c[256];
			f.Text(d->id, c, d->value);
			backColor = ((backColor << 16) & 0xff0000) | ((backColor >> 16) & 0xff) | (backColor & 0xff00);
			wsprintf(mess, L"<%6x>%S", backColor, c);
			(*f.ptrMessage)(mess);
		}
	}
	static VOID CALLBACK WaitOrTimerCallback(PVOID lpParameter, BOOLEAN TimerOrWaitFired)
	{
		((Initialization *)lpParameter)->Do();
	}
public:
	Log::Inner inner;
	Log buffer;
	Initialization()
		: hTimer(NULL)
		, f(LogMess::FactoryMessages::Instance())
		, buffer(inner)
	{}
};
static Initialization initializationLog;

Log &Log::hidden = initializationLog.buffer;

void Log::Start()
{
	if(NULL == initializationLog.hTimer)
	{
		CreateTimerQueueTimer(&initializationLog.hTimer
		, NULL, Initialization::WaitOrTimerCallback
		, &initializationLog.buffer
		, 3000
		, 1000
		, WT_EXECUTEINTIMERTHREAD
		);
	}
}
