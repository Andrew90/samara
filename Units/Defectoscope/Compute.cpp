#include "stdafx.h"
#include "Compute.h"
#include "USPCData.h"
#include "Dialogs.h"
#include "MedianFiltre.h"
#include "DebugMess.h"
#include "AppBase.h"
#include "ResultData.h"
#include "SelectMessage.h"
/*
template<class T>void StatusZoneDefect(int offs, double data, int zone, double (&brakThreshold)[App::zonesCount], double (&klass2Threshold)[App::zonesCount], char &status)
{
	 if(data > brakThreshold[zone])
	 {
#pragma message("дописать")
		 status = StatusId<Clr<BorderDefect<T>>>();
	 }
	 else  if(data > klass2Threshold[zone])
	 {
#pragma message("дописать")
		 status = StatusId<Clr<BorderKlass2<T>>>();
	 }
	 else
	 {
		status = StatusId<Nominal>();
	 }
}
 */
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

#define and(_0, _1, _2) if(StatusId<_0>() == _min && StatusId<_1>() == _max) return StatusId<_2>()

#define or(_0, _1, _2) if(StatusId<_0>() == _min || StatusId<_1>() == _max) return StatusId<_2>()

int StatusZoneThickness(int _min, int _max)
{
  // or(DeathZone, DeathZone, DeathZone);
   /*
   and(Nominal  , AboveNorm, AboveNorm);
   and(Nominal  , BelowNorm, BelowNorm);

   and(AboveNorm, AboveNorm, AboveNorm);
   and(AboveNorm, BelowNorm, BelowAboveNorm);
   
   and(BelowNorm, BelowNorm, BelowNorm);
   and(BelowNorm, AboveNorm, BelowAboveNorm);
   */

   //if(StatusId<Nominal>() == _min)
   //{
	//   if(StatusId<Nominal	     >() == _max)	return 	StatusId<Nominal       >();
	//   if(StatusId<AboveNorm	 >() == _max)	return 	StatusId<AboveNorm     >();
	//   if(StatusId<BelowNorm	 >() == _max)	return 	StatusId<BelowNorm     >();
	//   if(StatusId<BelowAboveNorm>() == _max)	return 	StatusId<BelowAboveNorm>();
   //}
   //
   //if(StatusId<AboveNorm>() == _min)
   //{
	//   if(StatusId<Nominal	     >() == _max)	return 	StatusId<AboveNorm     >();
	//   if(StatusId<AboveNorm	 >() == _max)	return 	StatusId<AboveNorm     >();
	//   if(StatusId<BelowNorm	 >() == _max)	return 	StatusId<BelowAboveNorm>();
	//   if(StatusId<BelowAboveNorm>() == _max)	return 	StatusId<BelowAboveNorm>();
   //}
   //
   //if(StatusId<BelowNorm>() == _min)
   //{
	//   if(StatusId<Nominal	     >() == _max)	return 	StatusId<BelowNorm     >();
	//   if(StatusId<AboveNorm	 >() == _max)	return 	StatusId<BelowAboveNorm>();
	//   if(StatusId<BelowNorm	 >() == _max)	return 	StatusId<BelowNorm     >();
	//   if(StatusId<BelowAboveNorm>() == _max)	return 	StatusId<BelowAboveNorm>();
   //}
   //
   //or(BelowNorm, BelowNorm, BelowNorm);
   //or(AboveNorm, AboveNorm, AboveNorm);
   //or(Nominal  , Nominal  , Nominal);

   return _min;
}

template<class T>int StatusZoneDefect(int _min, int _max)
{
	 //or(DeathZone     , DeathZone     , DeathZone);
	 //or(Defect        , Defect        , Defect);
	 //or(Treshold2Class, Treshold2Class, Treshold2Class);
	 //or(Nominal       , Nominal       , Nominal);

	 return _min;
}

int StatusZoneCommon(int _min, int _max)
{
	 //or(DeathZone     , DeathZone     , DeathZone);
	 //
	 //if(StatusId<Treshold2Class>() == _max)
	 //{
	//	 if(StatusId<Nominal	   >() == _min)	return 	StatusId<Treshold2Class              >();
	//	 if(StatusId<AboveNorm	   >() == _min)	return 	StatusId<Treshold2ClassAboveNorm     >();
	//	 if(StatusId<BelowNorm	   >() == _min)	return 	StatusId<Treshold2ClassBelowNorm     >();
	//	 if(StatusId<BelowAboveNorm>() == _min)	return 	StatusId<Treshold2ClassBelowAboveNorm>();
	 //}
	 //
	 //if(StatusId<Defect>() == _max)
	 //{
	//	 if(StatusId<Nominal	   >() == _min)	return 	StatusId<Defect              >();
	//	 if(StatusId<AboveNorm	   >() == _min)	return 	StatusId<DefectAboveNorm     >();
	//	 if(StatusId<BelowNorm	   >() == _min)	return 	StatusId<DefectBelowNorm     >();
	//	 if(StatusId<BelowAboveNorm>() == _min)	return 	StatusId<DefectBelowAboveNorm>();
	 //}
	 //
	 //if(StatusId<Nominal>() == _max)
	 //{
	//	 if(StatusId<Nominal	   >() == _min)	return 	StatusId<Nominal       >();
	//	 if(StatusId<AboveNorm	   >() == _min)	return 	StatusId<AboveNorm     >();
	//	 if(StatusId<BelowNorm	   >() == _min)	return 	StatusId<BelowNorm     >();
	//	 if(StatusId<BelowAboveNorm>() == _min)	return 	StatusId<BelowAboveNorm>();
	 //}

	 return _min;
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
			//char t = d.status[0][i];
			for(int j = 0; j < App::count_sensors; ++j)
			{
				//t = StatusZoneDefect<Data>(d.status[j][i], t);
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
		for(int i = 0; i < d.currentOffsetZones; ++i)
		{
			d.commonStatus[i] = StatusZoneThickness(d.statusMin[i], d.statusMax[i]);
		}
	}

	void UndefinedItem(USPCViewerData &x)
	{
		ZeroMemory(x.status, sizeof(x.status));
		ZeroMemory(x.buffer, sizeof(x.buffer));
	}

	void UndefinedItem(USPCViewerThicknessData &x)
	{
		ZeroMemory(x.bufferMax, sizeof(x.bufferMax));
		ZeroMemory(x.bufferMin, sizeof(x.bufferMin));
		ZeroMemory(x.commonStatus, sizeof(x.commonStatus));
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

		//for(int i = 0; i < App::zonesCount; ++i)
		//{
		//	char thick = StatusId<Clr<Undefined>>();
		//	if(thicknessOnJob) thick = StatusZoneThickness(thicknessStatus[i], thick);
		//
		//	char def = StatusId<Clr<Undefined>>();
		//	if(crossOnJob    ) def = StatusZoneDefect<Cross>(crossStatus[i]    , def);
		//	if(longOnJob     ) def = StatusZoneDefect<Long>(longStatus[i]     , def);
		//
		//	char t = StatusZoneCommon(thick, def);
		//	
		//	if(StatusId<Clr<Undefined>>() == t) break;
		//
		//	resultStatus[i] = t;
		//
		//	++currentOffset;
		//}

		int buf[4];

		for(int i = 0; i < App::zonesCount; ++i)
		{
			//char thick = StatusId<Clr<Undefined>>();
			//if(thicknessOnJob) thick = StatusZoneThickness(thicknessStatus[i], thick);
			//
			//char def = StatusId<Clr<Undefined>>();
			//if(crossOnJob    ) def = StatusZoneDefect<Cross>(crossStatus[i]    , def);
			//if(longOnJob     ) def = StatusZoneDefect<Long>(longStatus[i]     , def);
			//
			//char t = StatusZoneCommon(thick, def);
			//
			//if(StatusId<Clr<Undefined>>() == t) break;
			int k = 0;
			memset(buf, -1, sizeof(buf));

			if(crossOnJob    ) buf[k++] = crossStatus[i]    ;
			if(longOnJob     ) buf[k++] = longStatus[i]     ;
			if(thicknessOnJob) buf[k++] = thicknessStatus[i];
			
			int t = 0;

			SelectMessage(buf, t);

			resultStatus[i] = t;
		
			++currentOffset;
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