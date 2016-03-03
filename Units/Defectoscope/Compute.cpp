#include "stdafx.h"
#include "Compute.h"
#include "USPCData.h"
#include "Dialogs.h"
#include "MedianFiltre.h"
#include "DebugMess.h"

void StatusZoneDefect(int offs, double data, int zone, double (&brakThreshold)[App::zonesCount], double (&klass2Threshold)[App::zonesCount], char &status)
{
	 if(data > brakThreshold[zone])
	 {
		 status = StatusId<Defect>();
	 }
	 else  if(data > klass2Threshold[zone])
	 {
		 status = StatusId<Treshold2Class>();
	 }
	 else
	 {
		status = StatusId<Nominal>();
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
	template<class T>void ComputeData(USPCViewerData &d, MedianFiltre (&f)[App::count_sensors]
	, double (&brakThreshold)[App::zonesCount], double (&klass2Threshold)[App::zonesCount])
	{
		USPC7100_ASCANDATAHEADER *b = d.ascanBuffer;
		ZeroMemory(d.buffer, sizeof(d.buffer));
		T filtre(f);
		for(int i = 0; i < d.currentOffsetZones; ++i)
		{
			for(int j = 0; j < dimention_of(d.buffer); ++j)
			{
				d.buffer[j][i] = -1;;
				d.status[j][i] = StatusId<Undefined>();
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
						StatusZoneDefect(j, t, i, brakThreshold, klass2Threshold, d.status[channel][i]);
					}
				}				
			}
		}
	}

	void ComputeData(USPCViewerThicknessData &d, MedianFiltre (&f)[App::count_sensors])
	{
		USPC7100_ASCANDATAHEADER *b = d.ascanBuffer;
		ZeroMemory(d.zonesMax, sizeof(d.bufferMax));

		for(int i = 0; i < App::zonesCount; ++i)
			d.zonesMin[i] = 1000;

		for(int i = 0; i < d.currentOffsetZones; ++i)
		{
			double x = 0.1 * (rand() % 120) + 3; 
			if(d.zonesMax[i] < x)
			{
				d.zonesMax[i] = x;
			}
			if(d.zonesMin[i] > x)
			{
				d.zonesMin[i] = x * 0.7;
			}
			d.commonStatus[i] = rand() % 6 + 1;
		}
	}

	template<class O, class P>struct __recalculation__
	{
		void operator()(O *, P *)
		{

			MedianFiltre f[App::count_sensors];
			if(Singleton<MedianFiltreTable>::Instance().items.get<MedianFiltreOn<O> >().value)
			{
			int width = Singleton<MedianFiltreTable>::Instance().items.get<MedianFiltreWidth<O> >().value;
			width |= 1;
			for(int i = 0; i < dimention_of(f); ++i) f[i].Clear(width);
			ComputeData<FiltreOn>(Singleton<ItemData<O> >::Instance()
				, f
				, Singleton<ThresholdsTable>::Instance().items.get<BorderDefect<O> >().value
				, Singleton<ThresholdsTable>::Instance().items.get<Border2Class<O> >().value
				);
			}
			else
			{
				ComputeData<FiltreOff>(Singleton<ItemData<O> >::Instance()
				, f
				, Singleton<ThresholdsTable>::Instance().items.get<BorderDefect<O> >().value
				, Singleton<ThresholdsTable>::Instance().items.get<Border2Class<O> >().value
				);
			}
		}
	};
}

void Compute::Recalculation()
{	
	typedef TL::MkTlst<Cross, Long/*, Thickness*/>::Result list;
	TL::foreach<list, __recalculation__>()((TL::Factory<list> *)0, (int *)0);
	app.MainWindowUpdate();
}

Compute compute;

void RecalculationDlg::Do(HWND)
{
	compute.Recalculation();
}