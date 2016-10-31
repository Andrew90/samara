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
#include "LogMessages.h"
#include "LogBuffer.h"

void StatusZoneThickness(int offs, double &data, int zone, double (&maxThreshold)[App::count_zones]
, double (&minThreshold)[App::count_zones], double (&nominalTreshold)[App::count_zones], char &status)
{
	double min = nominalTreshold[zone] - minThreshold[zone];
	double max = nominalTreshold[zone] + maxThreshold[zone];
	if(0.0 == data)
	{
		status = StatusId<Clr<Undefined>>();
	}
	else if(data < min)
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
		inline double AddX(int i, double data)
		{
			MedianFiltre &ff = f[i];
			return ff.bufX[ff.AddX(data)];
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
		inline double AddX(int i, double data)
		{
			return data;
		}
	};
	template<class T, class Data>void ComputeData(USPCViewerData &d, MedianFiltre (&f)[App::count_sensors]
	, double (&brakThreshold)[App::count_zones], double (&klass2Threshold)[App::count_zones])
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
			for(int jj = d.offsets[i], last = d.offsets[i + 1]; jj < last; ++jj)
			{
				WORD channel = b[jj].Channel;
				if(channel < App::count_sensors)
				{
					//��������� �������� �� ������������ ������
					int j = jj + d.offsSensor[channel];					
					if(j > 0)
					{
						if(channel != b[j].Channel)
						{
							for(int k = j + 1, len = j + App::count_sensors + 2; k < len; ++k)
							{
								if(b[k].Channel == channel)
								{
									j = k;
									break;
								}
							}
						}
						double t = filtre(channel, b[j].hdr.G1Amp);
						int z = jj / App::count_sensors;
						z *= App::count_sensors;
						if(z < d.deadZoneSamplesBeg || z > d.deadZoneSamplesEnd)
						{
							if(StatusId<Clr<Undefined>>() == d.status[channel][i])
							{
								d.status[channel][i] = StatusId<Clr<DeathZone>>();
							}
						}
						else
						{							
							if(t > d.buffer[channel][i])
							{
								d.buffer[channel][i] = t;						
								StatusZoneDefect<Data>(j, t, i, brakThreshold, klass2Threshold, d.status[channel][i]);
							}
						}
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
	, double (&normThickness)[App::count_zones], double (&minThickness)[App::count_zones], double (&maxThickness)[App::count_zones])
	{
		USPC7100_ASCANDATAHEADER *b = d.ascanBuffer;
		T filtre(f);
		for(int i = 0; i < d.currentOffsetZones; ++i)
		{
			d.bufferMin[i] = 1000;
			d.bufferMax[i] = -1;
			d.statusMin[i] = StatusId<Clr<Undefined>>();
			d.statusMax[i] = StatusId<Clr<Undefined>>();
			for(int jj = d.offsets[i], last = d.offsets[i + 1]; jj < last; ++jj)
			{
				WORD channel = b[jj].Channel;	
				if(channel < App::count_sensors)
				{
					//��������� �������� �� ������������ ������
					int j = jj + d.offsSensor[channel];					
					if(j > 0)
					{
						if(channel != b[j].Channel)
						{
							for(int k = j + 1, len = j + App::count_sensors + 2; k < len; ++k)
							{
								if(b[k].Channel == channel)
								{
									j = k;
									break;
								}
							}
						}
						double val = 2.5e-6 * b[j].hdr.G1Tof * d.scope_velocity[channel];
						double t = filtre(channel, val);
						int z = jj / App::count_sensors;
						z *= App::count_sensors;
						if(z < d.deadZoneSamplesBeg || z > d.deadZoneSamplesEnd)
						{
							if(StatusId<Clr<Undefined>>() == d.statusMax[i]
						//	|| StatusId<Clr<Undefined>>() == d.statusMin[i]
							)
							{
								d.statusMax[i] = StatusId<Clr<DeathZone>>();
								d.statusMin[i] = StatusId<Clr<DeathZone>>();
							}
						}
						else
						{													
							if(t > d.bufferMax[i])
							{													
								StatusZoneThickness(j, t, i, normThickness, minThickness, maxThickness, d.statusMax[i]);
								d.bufferMax[i] = t;
							}
							if(0 == val) val = 999999;
							t = filtre.AddX(channel, val);	
							if(999999 == t) t = 0;
							if(0 != t &&  t < d.bufferMin[i])
							{
								d.bufferMin[i] = t;	
								StatusZoneThickness(j, t, i, normThickness, minThickness, maxThickness, d.statusMin[i]);							
							}
						}
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
		void operator()()
		{
			ItemData<O> &data = Singleton<ItemData<O> >::Instance();
			if(Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<O> >().value)
			{
				data.SamplesPerZone(compute.lengthTube
					, Singleton<DeadAreaTable>::Instance().items.get<DeadAreaMM0<O>>().value
					, Singleton<DeadAreaTable>::Instance().items.get<DeadAreaMM1<O>>().value
					);
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
		void operator()()
		{
			ItemData<O> &data = Singleton<ItemData<O> >::Instance();
			if(Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<O> >().value)
			{
				data.SamplesPerZone(compute.lengthTube
					, Singleton<DeadAreaTable>::Instance().items.get<DeadAreaMM0<O>>().value
					, Singleton<DeadAreaTable>::Instance().items.get<DeadAreaMM1<O>>().value
					);
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

	template<class T>struct ResultOk
	{
		bool operator()(){return true;}
	};

	template<class T>struct ResultOk<BorderLower<T>>
	{
		bool operator()(){return false;}
	};
	template<class T>struct ResultOk<BorderDefect<T>>
	{
		bool operator()(){return false;}
	};
	
	template<class T>struct __first__;
	template<class A, class B, class C, class D>struct __first__<Clr<A, B, C, D>>
	{
		typedef A Result;
	};
	template<class O, class P>struct __result_ok__
	{
		bool operator()(P *p)
		{
			if(TL::IndexOf<label_message_list, O>::value == *p)
			{
				return ResultOk<typename __first__<O>::Result>()();
			}
			return true;
		}
	};

	bool TubeResult(char (&res)[App::count_zones], int count)
	{
		if(count > dimention_of(res)) count = dimention_of(res);
		for(int i = 0; i < count; ++i)
		{
			if(!TL::find<label_message_list, __result_ok__>()(&res[i])) return false;
		}
		return true;
	}

	void CommonStatus(bool &tubeResult)
	{
		bool crossOnJob     = Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<Cross> >().value;
		bool longOnJob      = Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<Long> >().value;
		bool thicknessOnJob = Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<Thickness> >().value;

		char (&crossStatus)[App::count_zones] = Singleton<ItemData<Cross> >::Instance().commonStatus;
		char (&longStatus)[App::count_zones] = Singleton<ItemData<Long> >::Instance().commonStatus;
		char (&thicknessStatus)[App::count_zones] = Singleton<ItemData<Thickness> >::Instance().commonStatus;

		char (&resultStatus)[App::count_zones] = Singleton<ResultViewerData>::Instance().commonStatus;
		int &currentOffset = Singleton<ResultViewerData>::Instance().currentOffsetZones;

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
			if(thicknessOnJob) buf[k  ] = thicknessStatus[i];
			
			int t = 0;

			SelectMessage(buf, t);

			resultStatus[i] = t;
		}
		tubeResult = TubeResult(resultStatus, currentOffset);
	}

}

template<class O, class P>struct __collection_data_ok__
{
	bool operator()()
	{
        if(Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<O> >().value)
		{
			if(0 == Singleton<ItemData<O> >::Instance().currentOffsetFrames)
			{
                Log::Mess<LogMess::AlarmNoDataCollection>();
				return false;
			}
		}
		return true;
	}
};

template<class O, class P>struct __buffer_over_flow__
{
	bool operator()()
	{
		if(Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<O> >().value)
		{
			int count = Singleton<ItemData<O> >::Instance().currentOffsetFrames;
			zprint("overflow %d  %d\n", App::count_frames, count);
			if(App::count_frames > count) return true;
			Log::Mess<LogMess::AlarmBufferOverflow>();
			return false;
		}
		return true;
	}
};

template<class O, class P>struct __min_size__
{
	void operator()(P &p)
	{
		if(Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<O> >().value)
		{
			if(0 == p)
			{
				p = Singleton<ItemData<O> >::Instance().currentOffsetFrames;
			}
			else if(p > Singleton<ItemData<O> >::Instance().currentOffsetFrames)
			{
				p = Singleton<ItemData<O> >::Instance().currentOffsetFrames;
			}
		}
	}
};

template<class O, class P>struct __set_size__
{
	void operator()(P &p)
	{
		if(Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<O> >().value)
		{
			Singleton<ItemData<O> >::Instance().currentOffsetFrames = p;
		}
	}
};

void Compute::Recalculation()
{	
	bool dataOk = TL::find<USPC::items_list, __collection_data_ok__>()();//�������� �� ������� ������ � ������
	bool bufferNotOverflow = TL::find<USPC::items_list, __buffer_over_flow__>()();
	if(!bufferNotOverflow) return; //�������� �� ������������ ������
	int minSize = 0;
	TL::foreach<USPC::items_list, __min_size__>()(minSize);
	minSize /= App::count_sensors;
	minSize *= App::count_sensors;
	TL::foreach<USPC::items_list, __set_size__>()(minSize);
	TL::foreach<USPC::items_list, __recalculation__>()();
	CommonStatus(tubeResult);
	if(dataOk)
	{		
		double len = 0.001 * lengthTube;
		if(tubeResult)
		{
			Log::Mess<LogMess::CycleOk>(len);
		}
		else
		{
			Log::Mess<LogMess::CycleBrak>(len);
		}
	}
	app.MainWindowUpdate();
}

void Compute::LengthTube(unsigned startTime, unsigned baseTime, unsigned stopTime)
{
   double offs = Singleton<AdditionalSettingsTable>::Instance().items.get<ReferenceOffset1>().value; 
   lengthTube = int(offs * (stopTime - startTime) /(baseTime - startTime));
   lengthTube -= App::lengthCaretka;
}

Compute compute;

void RecalculationDlg::Do(HWND)
{
	compute.Recalculation();
}