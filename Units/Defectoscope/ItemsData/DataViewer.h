#pragma once
#include "App.h"
#include "DebugMess.h"

struct DefectData
{
	double *data;
	USPC7100_ASCANDATAHEADER **scan;
	int count;
	DefectData();
	void Set(int start, int stop, int channel, int offs, int maxOffs, USPC7100_ASCANDATAHEADER *s);
	void Drop();
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
	   Set(start, stop, channel, offs, maxOffs, s);
	}
};