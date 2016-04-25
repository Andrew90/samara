#include "stdafx.h"
#include "Compute.h"
#include "USPCData.h"
#include "Dialogs.h"
#include "MedianFiltre.h"
#include "DebugMess.h"
#include "AppBase.h"
#include "ResultData.h"
#include "SelectMessage.h"
#include "LabelMessage.h"

void StatusZoneThickness(int offs, double data, int zone, double (&maxThreshold)[App::zonesCount]
    , double (&minThreshold)[App::zonesCount], double (&nominalTreshold)[App::zonesCount], char &status)
{
	double min = nominalTreshold[zone] - minThreshold[zone];
	double max = nominalTreshold[zone] + maxThreshold[zone];
	 if(data < min)
	 {
		 status = StatusId<Clr<BorderLower<Thickness> >>();
	 }
	 else  if(data > max)
	 {
		 status = StatusId<Clr<BorderAbove<Thickness> >>();
	 }
	 else
	 {
		status = StatusId<Clr<Nominal>>();
	 }
}

Compute::Compute()
{
}

namespace
{
	struct FiltreOn
	{
		 MedianFiltre (&f)[App::count_sensors];
		FiltreOn( MedianFiltre (&f)[App::count_sensors])
			: f(f)
		{}
		inline double operator()(int i, double data)
		{
			MedianFiltre &ff = f[i];
			return ff.buf[ff.Add(data)];
		}
	};
	struct FiltreOff
	{
		FiltreOff( MedianFiltre (&f)[App::count_sensors])
		{}
		inline double operator()(int i, double data)
		{
			return data;
		}
	};
	template<class T, class Data>void ComputeData(USPCViewerData &d, MedianFiltre (&f)[App::count_sensors]
	, double (&brakThreshold)[App::zonesCount], double (&klass2Threshold)[App::zonesCount])
	{
		USPC7100_ASCANDATAHEADER *b = d.ascanBuffer;
		T filtre(f);
		for(int i = 0; i < d.currentOffsetZones; ++i)
		{
			for(int j = 0; j < dimention_of(d.buffer); ++j)
			{
				d.buffer[j][i] = -1;
				d.status[j][i] = StatusId<Clr<Undefined>>();
			}
			for(int j = d.offsets[i], last = d.offsets[i + 1]; j < last; ++j)
			{
				WORD channel = b[j].Channel;
				if(channel < App::count_sensors)
				{
					double t = filtre(channel, b[j].hdr.G1Amp);
					if(t > d.buffer[channel][i])
					{
						d.buffer[channel][i] = t;						
						StatusZoneDefect<Data>(j, t, i, brakThreshold, klass2Threshold, d.status[channel][i]);
					}
				}				
			}
		}
		int buf[ App::count_sensors + 1];
		buf[App::count_sensors] = -1;
		for(int i = 0; i < d.currentOffsetZones; ++i)
		{
			for(int j = 0; j < App::count_sensors; ++j)
			{
				buf[j] = d.status[j][i];
			}
			int t = 0;
		    SelectMessage(buf, t);
			d.commonStatus[i] = t;
		}
	}

	template<class T, class Data>void ComputeData(USPCViewerThicknessData &d, MedianFiltre (&f)[App::count_sensors]
	, double (&normThickness)[App::zonesCount], double (&minThickness)[App::zonesCount], double (&maxThickness)[App::zonesCount])
	{
		USPC7100_ASCANDATAHEADER *b = d.ascanBuffer;

		T filtre(f);
		for(int i = 0; i < d.currentOffsetZones; ++i)
		{
			d.bufferMin[i] = 1000;
			d.bufferMax[i] = -1;
			d.statusMin[i] = StatusId<Clr<Undefined>>();
			d.statusMin[i] = StatusId<Clr<Undefined>>();
			for(int j = d.offsets[i], last = d.offsets[i + 1]; j < last; ++j)
			{
				WORD channel = b[j].Channel;				
				if(channel < App::count_sensors)
				{
					double t = filtre(channel, b[j].hdr.G1Amp);
					if(t > d.bufferMax[i])
					{
						d.bufferMax[i] = t;						
						StatusZoneThickness(j, t, i, normThickness, minThickness, maxThickness, d.statusMax[i]);
					}
					if(t < d.bufferMin[i])
					{
						d.bufferMin[i] = t;						
						StatusZoneThickness(j, t, i, normThickness, minThickness, maxThickness, d.statusMin[i]);
					}
				}				
			}
		}
		
		int buf[3];
		buf[2] = -1;
		for(int i = 0; i < d.currentOffsetZones; ++i)
		{			
			buf[0] = d.statusMin[i];		
			buf[1] = d.statusMax[i];
			int t = 0;
			SelectMessage(buf, t);
			d.commonStatus[i] = t;
		}
	}

	void UndefinedItem(USPCViewerData &x)
	{
		ZeroMemory(x.status, sizeof(x.status));
		ZeroMemory(x.buffer, sizeof(x.buffer));
		x.currentOffsetZones = 0;
	}

	void UndefinedItem(USPCViewerThicknessData &x)
	{
		ZeroMemory(x.bufferMax, sizeof(x.bufferMax));
		ZeroMemory(x.bufferMin, sizeof(x.bufferMin));
		ZeroMemory(x.commonStatus, sizeof(x.commonStatus));
		x.currentOffsetZones = 0;
	}

	template<class O, class P>struct __recalculation__
	{
		void operator()(O *, P *)
		{
			ItemData<O> &data = Singleton<ItemData<O> >::Instance();
			if(Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<O> >().value)
			{
				MedianFiltre f[App::count_sensors];
				if(Singleton<MedianFiltreTable>::Instance().items.get<MedianFiltreOn<O> >().value)
				{
					int width = Singleton<MedianFiltreTable>::Instance().items.get<MedianFiltreWidth<O> >().value;
					width |= 1;
					for(int i = 0; i < dimention_of(f); ++i) f[i].Clear(width);
					ComputeData<FiltreOn, O>(data
						, f
						, Singleton<ThresholdsTable>::Instance().items.get<BorderDefect<O> >().value
						, Singleton<ThresholdsTable>::Instance().items.get<BorderKlass2<O> >().value
						);
				}
				else
				{
					ComputeData<FiltreOff, O>(data
						, f
						, Singleton<ThresholdsTable>::Instance().items.get<BorderDefect<O> >().value
						, Singleton<ThresholdsTable>::Instance().items.get<BorderKlass2<O> >().value
						);
				}
			}
			else
			{
				UndefinedItem(data);
			}
		}
	};
	
	template<class P>struct __recalculation__<Thickness, P>
	{
		typedef Thickness O;
		void operator()(O *, P *)
		{
			ItemData<O> &data = Singleton<ItemData<O> >::Instance();
			if(Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<O> >().value)
			{
				MedianFiltre f[App::count_sensors];
				if(Singleton<MedianFiltreTable>::Instance().items.get<MedianFiltreOn<O> >().value)
				{
					int width = Singleton<MedianFiltreTable>::Instance().items.get<MedianFiltreWidth<O> >().value;
					width |= 1;
					for(int i = 0; i < dimention_of(f); ++i) f[i].Clear(width);
					ComputeData<FiltreOn, O>(Singleton<ItemData<O> >::Instance()
						, f
						, Singleton<ThresholdsTable>::Instance().items.get<BorderAbove<O> >().value
						, Singleton<ThresholdsTable>::Instance().items.get<BorderLower<O> >().value
						, Singleton<ThresholdsTable>::Instance().items.get<BorderNominal<O> >().value
						);
				}
				else
				{
					ComputeData<FiltreOff, O>(Singleton<ItemData<O> >::Instance()
						, f
						, Singleton<ThresholdsTable>::Instance().items.get<BorderAbove<O> >().value
						, Singleton<ThresholdsTable>::Instance().items.get<BorderLower<O> >().value
						, Singleton<ThresholdsTable>::Instance().items.get<BorderNominal<O> >().value
						);
				}
			}
			else
			{
				UndefinedItem(data);
			}
		}
	};

	void CommonStatus()
	{
		bool crossOnJob     = Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<Cross> >().value;
		bool longOnJob      = Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<Long> >().value;
		bool thicknessOnJob = Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<Thickness> >().value;

		char (&crossStatus)[App::zonesCount] = Singleton<ItemData<Cross> >::Instance().commonStatus;
		char (&longStatus)[App::zonesCount] = Singleton<ItemData<Long> >::Instance().commonStatus;
		char (&thicknessStatus)[App::zonesCount] = Singleton<ItemData<Thickness> >::Instance().commonStatus;

		char (&resultStatus)[App::zonesCount] = Singleton<ResultViewerData>::Instance().commonStatus;
		int &currentOffset = Singleton<ResultViewerData>::Instance().currentOffset;

		ZeroMemory(resultStatus, sizeof(currentOffset));

		currentOffset = 0;

		if(crossOnJob) if(currentOffset < Singleton<ItemData<Cross> >::Instance().currentOffsetZones) currentOffset = Singleton<ItemData<Cross> >::Instance().currentOffsetZones;
		if(longOnJob) if(currentOffset < Singleton<ItemData<Long> >::Instance().currentOffsetZones) currentOffset = Singleton<ItemData<Long> >::Instance().currentOffsetZones;
		if(thicknessOnJob) if(currentOffset < Singleton<ItemData<Thickness> >::Instance().currentOffsetZones) currentOffset = Singleton<ItemData<Thickness> >::Instance().currentOffsetZones;

		int buf[4];

		for(int i = 0; i < currentOffset; ++i)
		{
			int k = 0;
			memset(buf, -1, sizeof(buf));

			if(crossOnJob    ) buf[k++] = crossStatus[i]    ;
			if(longOnJob     ) buf[k++] = longStatus[i]     ;
			if(thicknessOnJob) buf[k++] = thicknessStatus[i];
			
			int t = 0;

			SelectMessage(buf, t);

			resultStatus[i] = t;
		}
	}
}

void Compute::Recalculation()
{	
	typedef TL::MkTlst<Cross, Long, Thickness>::Result list;
	TL::foreach<list, __recalculation__>()((TL::Factory<list> *)0, (int *)0);

	CommonStatus();

	app.MainWindowUpdate();
}

Compute compute;

void RecalculationDlg::Do(HWND)
{
	compute.Recalculation();
}

/*
void Compute::Test()
{
	int buf[4];
	buf[3] = -1;

	buf[2] = 10;//TL::IndexOf<label_message_list, Clr<Undefined>>::value;
	buf[0] = 5;//TL::IndexOf<label_message_list, Clr<BorderDefect<Long>>>::value;
	buf[1] = 2;//TL::IndexOf<label_message_list, Clr<BorderAbove<Thickness>>>::value;

	int t = 0;

	SelectMessage(buf, t);
	dprint("mess %d\n", t);
}
*/