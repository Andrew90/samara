#pragma once
#include "App.h"
#include "DebugMess.h"

struct DefectData
{
	double data[2048];
	USPC7100_ASCANDATAHEADER *scan[2048];
	char status[2048];
	int count;
	int zone;
	int &medianFiltreWidth;
	bool &medianFiltreOn;
	DefectData();
	void Set(int zone, int start, int stop, int channel, int offs, int maxOffs, USPC7100_ASCANDATAHEADER *s);
};

template<class T, int channel>struct DataViewer: DefectData
{
	void Do(int zone)
	{
	   ItemData<T> &d = Singleton<ItemData<T> >::Instance();
	   int start = d.offsets[zone];
	   int stop = d.offsets[1 + zone];
	   int offs = Singleton<OffsetsTable>::Instance().items.get<Offset<T, channel> >().value;
	   int maxOffs = d.currentOffsetFrames;
	   USPC7100_ASCANDATAHEADER *s = d.ascanBuffer;
	   Set(zone, start, stop, channel, offs, maxOffs, s);
	}
};