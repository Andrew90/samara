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
	double (&brackThreshold )[App::zonesCount];
	double (&klass2Threshold)[App::zonesCount];
	DefectData(int &, bool &, double (&)[App::zonesCount], double (&)[App::zonesCount]);
	void Set(int zone, int start, int stop, int channel, int offs, int maxOffs, USPC7100_ASCANDATAHEADER *s);
};

template<class T, int channel>struct DataViewer: DefectData
{
	DataViewer()
		: DefectData(
		   Singleton<MedianFiltreTable>::Instance().items.get<MedianFiltreWidth<T> >().value
		   , Singleton<MedianFiltreTable>::Instance().items.get<MedianFiltreOn<T> >().value
		   , Singleton<ThresholdsTable>::Instance().items.get<BorderDefect<T> >().value
		   , Singleton<ThresholdsTable>::Instance().items.get<BorderKlass2<T> >().value
		)
	{}
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

struct ThicknessData
{
	double data[2048];
	USPC7100_ASCANDATAHEADER *scan[2048];
	char status[2048];
	int count;
	int zone;
	int &medianFiltreWidth;
	bool &medianFiltreOn;
	double (&aboveBorder  )[App::zonesCount];
	double (&lowerBorder  )[App::zonesCount];
	double (&nominalBorder)[App::zonesCount];
	ThicknessData(int &, bool &, double (&)[App::zonesCount], double (&)[App::zonesCount], double (&)[App::zonesCount]);
	void Set(int zone, int start, int stop, int channel, int offs, int maxOffs, USPC7100_ASCANDATAHEADER *s);
};
template<int channel>struct DataViewer<Thickness, channel>: ThicknessData
{
	typedef Thickness T;
	DataViewer()
		: ThicknessData(
		   Singleton<MedianFiltreTable>::Instance().items.get<MedianFiltreWidth<T> >().value
		   , Singleton<MedianFiltreTable>::Instance().items.get<MedianFiltreOn<T> >().value
		   , Singleton<ThresholdsTable>::Instance().items.get<BorderAbove  <T> >().value
		   , Singleton<ThresholdsTable>::Instance().items.get<BorderLower  <T> >().value
		   , Singleton<ThresholdsTable>::Instance().items.get<BorderNominal<T> >().value
		)
	{}
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