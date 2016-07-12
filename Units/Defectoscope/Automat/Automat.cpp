#include "stdafx.h"
#include "Automat.h"
#include <MMSystem.h>
#include "App.h"
#include "AppBase.h"
#include "Device1730.h"
#include "LogMessages.h"
#include "LogBuffer.h"
#include "LogMessageToTopLabel.h"
#include "AutomatAdditional.h"
#include "USPC.h"
#include "Compute.h"
#include "AppKeyHandler.h"
#include "MainWindow.h"
#include "LogUSPCWindow.h"
#include "Stored.h"

struct Automat::Impl
{
	struct ExceptionExitProc{};
	struct ExceptionStopProc{};
	struct ExceptionTimeOutProc{};
	struct ExceptionContinueProc{};
	struct ExceptionСontrolСircuitsOffProc{};
	struct ExceptionСycleOffProc{};
	struct Exception_USPC_DO_ERROR_Proc{};
	struct Exception_USPC_ERROR_Proc{};
	HANDLE hThread;
	Impl() 
	{}
	static Impl &Instance(){static Impl x; return x;};
	void Do();
	static DWORD WINAPI ProcDo(LPVOID)
	{
		Impl::Instance().Do();
		return 0;
	}
	void Init()
	{
		hThread = CreateThread(NULL, 0, ProcDo, NULL, 0, NULL);
	}
	void Suspend()
	{
		SuspendThread(hThread);
	}
	void Resume()
	{
		while(ResumeThread(hThread));
	}

	template<class>struct On{};
	template<class>struct Off{};
	template<class>struct Inv{};
	template<class>struct Proc{};
	template<class>struct Once{};
	template<class T>struct Ex;

#define EX(n)template<>struct Ex<Exception##n##Proc>\
	{\
	HANDLE &handle;\
	Ex() : handle(App::Programm##n##Event){}\
	};

	EX(Exit)
	EX(Stop)
	EX(Continue)

#undef EX

	 template<class list>struct ArrEvents
	 {
		 template<class O, class P>struct loc
		 {
			 void operator()(P *p)
			 {
				 p[TL::IndexOf<list, O>::value] = Ex<O>().handle;
			 }
		 };
		 template<class O, class P>struct ev
		 {
			 void operator()(P *p)
			 {
				if(TL::IndexOf<list, O>::value == *p) throw O();
			 }
		 };
		 template<class P>struct ev<ExceptionContinueProc, P>
		 {
			 void operator()(P *){}
		 };
		 HANDLE h[TL::Length<list>::value];
		 ArrEvents()
		 {
			 TL::foreach<list, loc>()(h);
		 }

		 void Throw(unsigned t)
		 {
			 TL::foreach<list, ev>()(&t);
			 return;
		 }

	 };

	template<class List, template<class>class T>struct Filt;
	template<class Head, class Tail, template<class>class T>struct Filt<Tlst<Head, Tail>, T>
	{
		typedef typename Filt<Tail, T>::Result Result;
	};
	template<class Head, class Tail, template<class>class T>struct Filt<Tlst<T<Head>, Tail>, T>
	{
		typedef Tlst<Head, typename Filt<Tail, T>::Result> Result;
	};
	template<template<class>class T>struct Filt<NullType, T>
	{
		typedef NullType Result;
	};

	template<class O, class P>struct __bits_0__
	{
		void operator()(O *o, P *p)
		{
			*p |= o->value;
		}
	};

	template<class List, class TList>struct __filtr__;
	template<class Head, class Tail, class TList>struct __filtr__<Tlst<Head, Tail>, TList>
	{
		typedef typename TL::_if<
			TL::TypeInList<TList, Head>::value
			, Tlst<Head, typename __filtr__<Tail, TList>::Result>
			, typename __filtr__<Tail, TList>::Result
		>::Result Result;
	};

	template<class TList>struct __filtr__<NullType, TList>
	{
		typedef NullType Result;
	};

	template<class List, template<class, class>class b>struct __sel_bits__
	{
		template<class O, class P>void operator()(O *o, P *p)
		{
			TL::foreach<List, b>()(o, p);
		}
	};
	template<template<class, class>class b>struct __sel_bits__<NullType, b>
	{
		template<class O, class P>void operator()(O *o, P *p)
		{
		}
	};
	
	template<class List>struct SelectBits
	{
		void operator()(unsigned &bits)
		{
			bits = 0;
			__sel_bits__<typename __filtr__<List, InputBitTable::items_list>::Result, __bits_0__>()
				(&Singleton<InputBitTable>::Instance().items, &bits);
		}
	};
	template<>struct SelectBits<NullType>
	{
		void operator()(unsigned &bits)
		{
		}
	};

	template<class List>struct BitsOut
	{
		void operator()(unsigned &bits)
		{
			bits = 0;
			__sel_bits__<typename __filtr__<List, OutputBitTable::items_list>::Result, __bits_0__>()
				(&Singleton<OutputBitTable>::Instance().items, &bits);
		}
	};
	template<>struct BitsOut<NullType>
	{
		void operator()(unsigned &bits)
		{
		}
	};

	template<class O, class P>struct __default_do__
	{
		void operator()(P *p)
		{
			O::Do(*p);
		}
	};

	template<class List>struct DefaultDo
	{
		void operator()(unsigned bits)
		{
			TL::foreach<List, __default_do__>()(&bits);
		}
	};
	template<>struct DefaultDo<NullType>
	{
		void operator()(unsigned){}
	};

	template<class O, class P>struct __once_do__
	{
		void operator()(O *, P *)
		{
			O::Do();
		}
	};

	template<class List>struct OnceDo
	{
		void operator()()
		{
			TL::foreach<List, __once_do__>()();
		}
	};
	template<>struct OnceDo<NullType>
	{
		void operator()(){}
	};

	

//Не убирать	template<class List>struct OR_Bits			   
//Не убирать	{
//Не убирать		unsigned operator()(unsigned delay = (unsigned)-1)
//Не убирать		{
//Не убирать			if((unsigned)-1 != delay) delay += GetTickCount();
//Не убирать			unsigned bitOn = 0, bitOff = 0, bitInv = 0;
//Не убирать			SelectBits<typename Filt<List, On>::Result>()(bitOn);
//Не убирать			SelectBits<typename Filt<List, Off>::Result>()(bitOff);
//Не убирать			SelectBits<typename Filt<List, Inv>::Result>()(bitInv);
//Не убирать
//Не убирать			typedef TL::Append<typename Filt<List, Ex>::Result, ExceptionExitProc>::Result exeption_list;
//Не убирать			ArrEvents<exeption_list> arrEvents;
//Не убирать			
//Не убирать			while(true)
//Не убирать			{
//Не убирать				unsigned ev = WaitForMultipleObjects(dimention_of(arrEvents.h), arrEvents.h, FALSE, 5);
//Не убирать				unsigned res = device1730_0.Read();
//Не убирать				((unsigned short *)&res)[1] = device1730_1.Read();
//Не убирать				if(WAIT_TIMEOUT == ev)
//Не убирать				{
//Не убирать					if(bitOn || bitOff)
//Не убирать					{
//Не убирать						unsigned t = res ^ bitInv;
//Не убирать						if((t & bitOn) || (bitOff & (t ^ bitOff))) 
//Не убирать						{
//Не убирать								OnceDo<typename Filt<List, Once>::Result>()();
//Не убирать								return res;
//Не убирать						}
//Не убирать					}
//Не убирать					if((((unsigned short *)&res)[1]) & sycle_ts)
//Не убирать					{
//Не убирать						dprint("ExceptionStopProc  %x\n", sycle_ts);
//Не убирать						throw ExceptionStopProc();
//Не убирать					}
//Не убирать					DefaultDo<typename Filt<List, Proc>::Result>()();
//Не убирать					if(GetTickCount() >= delay) throw ExceptionTimeOutProc();
//Не убирать				}
//Не убирать				else
//Не убирать				{
//Не убирать					arrEvents.Throw(ev - WAIT_OBJECT_0);
//Не убирать				}
//Не убирать			}
//Не убирать		}
//Не убирать	};

	

	template<class List>struct AND_Bits
	{
		unsigned operator()(unsigned delay = (unsigned)-1)
		{
			if((unsigned)-1 != delay) delay += GetTickCount();
			unsigned bitOn = 0, bitOff = 0, bitInv = 0;
			SelectBits<typename Filt<List, On>::Result>()(bitOn);
			SelectBits<typename Filt<List, Off>::Result>()(bitOff);
			SelectBits<typename Filt<List, Inv>::Result>()(bitInv);

			typedef TL::Append<typename Filt<List, Ex>::Result, ExceptionExitProc>::Result exeption_list;
			ArrEvents<exeption_list> arrEvents;

			while(true)
			{
				unsigned ev = WaitForMultipleObjects(dimention_of(arrEvents.h), arrEvents.h, FALSE, 5);
				unsigned res = device1730.Read();
				if(WAIT_TIMEOUT == ev)
				{
					if(bitOn || bitOff)
					{						
						unsigned t = res ^ bitInv;
						if(bitOn == (t & (bitOn | bitOff))) 
						{
								OnceDo<typename Filt<List, Once>::Result>()();
								return res;
						}
					}					
					DefaultDo<typename Filt<List, Proc>::Result>()(res);
					if(GetTickCount() >= delay) throw ExceptionTimeOutProc();
				}
				else
				{
					arrEvents.Throw(ev - WAIT_OBJECT_0);
					return res;
				}
			}
		}
	};
	template<class List>struct OUT_Bits
	{
		void operator()()
		{
			unsigned bitOn, bitOff;
			bitOn = bitOff = 0;
			BitsOut<typename Filt<List, On>::Result>()(bitOn);
			BitsOut<typename Filt<List, Off>::Result>()(bitOff);

			unsigned res = device1730.ReadOutput();

            res &= ~bitOff;
			res |= bitOn;

			device1730.Write(res);
		}
	};
	template<class List>struct SET_Bits
	{
		void operator()()
		{
			unsigned res =0;
			BitsOut<typename Filt<List, On>::Result>()(res);

			device1730.Write(res);
		}
	};
///-----------------------------------------------------------------------------------
	template<>struct Off<iСontrolСircuits>
	{
		static unsigned &bit;
		static void Do(unsigned bits)
		{
		   if(0 == (bits & bit)) throw ExceptionСontrolСircuitsOffProc();
		}
	};
	
	template<>struct Off<iCycle>
	{
		static unsigned &bit;
		static void Do(unsigned bits)
		{
			 if(0 == (bits & bit)) throw ExceptionСycleOffProc();
		}
	};

	struct USPC_Do
	{
		static void Do(unsigned)
		{
			if(!USPC::Do()) Exception_USPC_DO_ERROR_Proc();
		}
	};
///-----------------------------------------------------------------------------------
};
unsigned &Automat::Impl::Off<iСontrolСircuits>::bit = Singleton<InputBitTable>::Instance().items.get<iСontrolСircuits>().value;
unsigned &Automat::Impl::Off<iCycle>::bit =	Singleton<InputBitTable>::Instance().items.get<iCycle>().value;

#define AND_BITS(...) AND_Bits<TL::MkTlst<__VA_ARGS__>::Result>()
#define OR_BITS(...) OR_Bits<TL::MkTlst<__VA_ARGS__>::Result>()

#define OUT_BITS(...) OUT_Bits<TL::MkTlst<__VA_ARGS__>::Result>()()
#define SET_BITS(...) SET_Bits<TL::MkTlst<__VA_ARGS__>::Result>()()
namespace
{
	 bool &onTheJobCross = Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<Cross>>().value;
	 bool &onTheJobLong = Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<Long>>().value;
	 bool &onTheJobThickness = Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<Thickness>>().value;
	 bool &viewInterrupt = 	Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<ViewInterrupt>>().value;
}

void Automat::Impl::Do()
{
	Log::Mess<LogMess::ProgramOpen>(0);
	LogMessageToTopLabel logMessageToTopLabel;
	try
	{
		while(true)
		{
			try
			{
				App::measurementOfRunning = false;				
				AND_BITS(Ex<ExceptionContinueProc>)();
				ResetEvent(App::ProgrammStopEvent);
				App::measurementOfRunning = true;
				Log::Mess<LogMess::WaitControlCircuitBitIn>();

				OUT_BITS(Off<oPowerBM>);

				AND_BITS(Ex<ExceptionStopProc>, On<iСontrolСircuits>)(10000);
				Log::Mess<LogMess::PowerBMOn>();

				OUT_BITS(On<oPowerBM>);
				Sleep(500);
				//проверить состояние трёх ультрозвуковых плат и мультиплексоров(через задержку)
				//Загрузить настройки для текущего типоразмера
				if(USPC::Open()) throw Exception_USPC_ERROR_Proc();
				//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
				AND_BITS(Ex<ExceptionStopProc>, On<iCycle>, On<iReady>, Proc<Off<iСontrolСircuits>>)(60 * 60 * 1000);
				SET_BITS(On<oPowerBM>);
				//подготовить ультрозвуковую систему к работе
				USPC::Start();
				//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
				OUT_BITS(On<oWork>);
				AND_BITS(Ex<ExceptionStopProc>, On<iControl>, Proc<Off<iCycle>>, Proc<Off<iСontrolСircuits>>)(60 * 60 * 1000);
				unsigned startTime = timeGetTime();
				//сбор данных с ультрозвуковых датчиков
				AND_BITS(Ex<ExceptionStopProc>, On<iBase>, Proc<Off<iCycle>>, Proc<Off<iСontrolСircuits>>, Proc<USPC_Do>)(60 * 60 * 1000);
				unsigned baseTime = timeGetTime();
				//вычислить скорость каретки и вывод на экран
				AutomatAdditional::ComputeSpeed(baseTime - startTime);
				//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
				AND_BITS(Ex<ExceptionStopProc>, Off<iControl>, Proc<Off<iCycle>>, Proc<Off<iСontrolСircuits>>, Proc<USPC_Do>)(60 * 60 * 1000);
				//Остановить плату USPC
				USPC::Stop();
				//расчёт данных, вывод на экран
				unsigned stopTime = timeGetTime();
				compute.LengthTube(startTime, baseTime, stopTime);
				compute.Recalculation();
				//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
				Sleep(100);
				OUT_BITS(Off<oWork>);
				//Режим прерывания
			    if(viewInterrupt)
				{
					ResetEvent(App::ProgrammContinueEvent);
					Log::Mess<LogMess::ContineCycle>();
					WaitForSingleObject(App::ProgrammContinueEvent, INFINITE);
				}
				//todo в зависимости от результатов контроля выставить сигналы РЕЗУЛЬТАТ1 и РЕЗУЛЬТАТ2
				OUT_BITS(On<oResult1>, On<oResult2>);
				Sleep(500);
				//выставить сигнал ПЕРЕКЛАДКА
				OUT_BITS(On<oToShiftThe>);
				//todo Записать результат контроля в базу данных
				Stored::Do();
				//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			}
			catch(ExceptionСontrolСircuitsOffProc)
			{
				ResetEvent(App::ProgrammContinueEvent);
				Log::Mess<LogMess::AlarmControlCircuts>();
				device1730.Write(0);
				//todo остановить сбор сканов
				AppKeyHandler::Stop();
			}	
			catch(ExceptionСycleOffProc)
			{
				ResetEvent(App::ProgrammContinueEvent);
				Log::Mess<LogMess::AlarmCycle>();
				SET_BITS(On<oPowerBM>);
				//todo остановить сбор сканов
				AppKeyHandler::Stop();
			}
			catch(ExceptionTimeOutProc)
			{
				ResetEvent(App::ProgrammContinueEvent);
				Log::Mess<LogMess::TimeoutPipe>();	
				SET_BITS(On<oPowerBM>);
				//todo остановить сбор сканов
				AppKeyHandler::Stop();
			}
			catch(ExceptionStopProc)
			{
				ResetEvent(App::ProgrammContinueEvent);
				Log::Mess<LogMess::InfoUserStop>();	
				SET_BITS(On<oPowerBM>);
				//todo остановить сбор сканов
				AppKeyHandler::Stop();
			}
			catch(Exception_USPC_DO_ERROR_Proc)
			{
				ResetEvent(App::ProgrammContinueEvent);
				Log::Mess<LogMess::TimeoutPipe>();	
				SET_BITS(On<oPowerBM>);
				//todo остановить сбор сканов
				AppKeyHandler::Stop();
			}
			catch(Exception_USPC_ERROR_Proc)
			{
				ResetEvent(App::ProgrammContinueEvent);
				Log::Mess<LogMess::AlarmUSPC>();	
				SET_BITS(On<oPowerBM>);
				//todo остановить сбор сканов
				AppKeyHandler::Stop();
				int ret = MessageBox(
					app.mainWindow.hWnd
					, L"Открыть окно просмотра сообщений?", L"Ошибка платы ултразвкового контроля"
					, MB_ICONEXCLAMATION | MB_YESNOCANCEL);
				if(IDYES == ret)
				{
					LogUSPCWindow::Open();
				}
			}
		}
	}
	catch(ExceptionExitProc)
	{
		CloseHandle(hThread);
		Log::Mess<LogMess::ProgramClosed>(0);
	}
}

 Automat::Automat()
	: impl(Automat::Impl::Instance())
{}

void Automat::Init()
{
	impl.Init();
}