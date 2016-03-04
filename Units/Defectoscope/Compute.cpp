#include "stdafx.h"
#include "Compute.h"
#include "USPCData.h"
#include "Dialogs.h"
#include "MedianFiltre.h"
#include "DebugMess.h"
#include "AppBase.h"

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

void StatusZoneThickness(int offs, double data, int zone, double (&maxThreshold)[App::zonesCount]
    , double (&minThreshold)[App::zonesCount], double (&nominalTreshold)[App::zonesCount], char &status)
{
	double min = nominalTreshold[zone] - minThreshold[zone];
	double max = nominalTreshold[zone] + maxThreshold[zone];
	 if(data < min)
	 {
		 status = StatusId<BelowNorm>();
	 }
	 else  if(data > max)
	 {
		 status = StatusId<AboveNorm>();
	 }
	 else
	 {
		status = StatusId<Nominal>();
	 }
}

#define and(_0, _1, _2) if(StatusId<_0>() == _min && StatusId<_1>() == _max) return StatusId<_2>()

#define or(_0, _1, _2) if(StatusId<_0>() == _min || StatusId<_1>() == _max) return StatusId<_2>()

int StatusZoneT(int _min, int _max)
{
   and(Nominal  , Nominal  , Nominal  );
   and(Nominal  , AboveNorm, AboveNorm);

   and(AboveNorm, AboveNorm, AboveNorm);
   
   and(BelowNorm, Nominal  , BelowNorm);
   and(BelowNorm, BelowNorm, BelowNorm);
   and(BelowNorm, AboveNorm, BelowAboveNorm);

   or(DeathZone, DeathZone, DeathZone);

   or(BelowNorm, BelowNorm, BelowNorm);
   or(AboveNorm, AboveNorm, AboveNorm);

   return StatusId<Undefined>();
}

#undef and
#undef or

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
		//ZeroMemory(d.buffer, sizeof(d.buffer));
		T filtre(f);
		for(int i = 0; i < d.currentOffsetZones; ++i)
		{
			for(int j = 0; j < dimention_of(d.buffer); ++j)
			{
				d.buffer[j][i] = -1;
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

	template<class T>void ComputeData(USPCViewerThicknessData &d, MedianFiltre (&f)[App::count_sensors]
	, double (&normThickness)[App::zonesCount], double (&minThickness)[App::zonesCount], double (&maxThickness)[App::zonesCount])
	{
		USPC7100_ASCANDATAHEADER *b = d.ascanBuffer;
		//ZeroMemory(d.zonesMax, sizeof(d.bufferMax));
		//
		//for(int i = 0; i < App::zonesCount; ++i)
		//	d.zonesMin[i] = 1000;
		T filtre(f);
		for(int i = 0; i < d.currentOffsetZones; ++i)
		{
			//for(int j = 0; j < dimention_of(d.buffer); ++j)
			//{
				d.bufferMin[i] = 1000;
				d.bufferMax[i] = -1;
				//d.status[i] = StatusId<Undefined>();
				d.statusMin[i] = StatusId<Undefined>();
				d.statusMin[i] = StatusId<Undefined>();
			//}
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
		for(int i = 0; i < d.currentOffsetZones; ++i)
		{
			d.status[i] = StatusZoneT(d.statusMin[i], d.statusMax[i]);
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
	
	template<class P>struct __recalculation__<Thickness, P>
	{
		typedef Thickness O;
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
					, Singleton<ThresholdsTable>::Instance().items.get<AboveBorder<O> >().value
					, Singleton<ThresholdsTable>::Instance().items.get<LowerBorder<O> >().value
					, Singleton<ThresholdsTable>::Instance().items.get<NominalBorder<O> >().value
					);
			}
			else
			{
				ComputeData<FiltreOff>(Singleton<ItemData<O> >::Instance()
					, f
					, Singleton<ThresholdsTable>::Instance().items.get<AboveBorder<O> >().value
					, Singleton<ThresholdsTable>::Instance().items.get<LowerBorder<O> >().value
					, Singleton<ThresholdsTable>::Instance().items.get<NominalBorder<O> >().value
					);
			}
		}
	};
}

void Compute::Recalculation()
{	
	typedef TL::MkTlst<Cross, Long, Thickness>::Result list;
	TL::foreach<list, __recalculation__>()((TL::Factory<list> *)0, (int *)0);
	app.MainWindowUpdate();
}

Compute compute;

void RecalculationDlg::Do(HWND)
{
	compute.Recalculation();
}