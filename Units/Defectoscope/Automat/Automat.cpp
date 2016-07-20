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
#include "RestartService.h"

struct Automat::Impl
{	
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
};
namespace
{
	struct ExceptionExitProc{};
	struct ExceptionStopProc{};
	struct ExceptionTimeOutProc{};
	struct ExceptionContinueProc{};
	struct ExceptionRunProc{};
	struct Exception�ontrol�ircuitsOffProc{};
	struct Exception�ycleOffProc{};
	struct Exception_USPC_DO_ERROR_Proc{};
	struct Exception_USPC_ERROR_Proc{};
	struct Exception_USPC_RestartService_ERROR_Proc{};

	//struct ExceptionTestRunProc{};

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
	EX(Run)
	//EX(TestRun)

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
			 bool operator()(P *p)
			 {
				if(TL::IndexOf<list, O>::value == *p) throw O();
				return true;
			 }
		 };
		 template<class P>struct ev<ExceptionRunProc, P>
		 {
			 typedef ExceptionRunProc O;
			 bool operator()(P *p)
			 {
				 return TL::IndexOf<list, O>::value != *p;
			 }
		 };
		 template<class P>struct ev<ExceptionContinueProc, P>
		 {
			 typedef ExceptionContinueProc O;
			 bool operator()(P *p)
			 {
				 return TL::IndexOf<list, O>::value != *p;
			 }
		 };
		 HANDLE h[TL::Length<list>::value];
		 ArrEvents()
		 {
			 TL::foreach<list, loc>()(h);
		 }

		 int Throw(unsigned t)
		 {
			 TL::find<list, ev>()(&t);
			 return t;
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

	

//�� �������	template<class List>struct OR_Bits			   
//�� �������	{
//�� �������		unsigned operator()(unsigned delay = (unsigned)-1)
//�� �������		{
//�� �������			if((unsigned)-1 != delay) delay += GetTickCount();
//�� �������			unsigned bitOn = 0, bitOff = 0, bitInv = 0;
//�� �������			SelectBits<typename Filt<List, On>::Result>()(bitOn);
//�� �������			SelectBits<typename Filt<List, Off>::Result>()(bitOff);
//�� �������			SelectBits<typename Filt<List, Inv>::Result>()(bitInv);
//�� �������
//�� �������			typedef TL::Append<typename Filt<List, Ex>::Result, ExceptionExitProc>::Result exeption_list;
//�� �������			ArrEvents<exeption_list> arrEvents;
//�� �������			
//�� �������			while(true)
//�� �������			{
//�� �������				unsigned ev = WaitForMultipleObjects(dimention_of(arrEvents.h), arrEvents.h, FALSE, 5);
//�� �������				unsigned res = device1730_0.Read();
//�� �������				((unsigned short *)&res)[1] = device1730_1.Read();
//�� �������				if(WAIT_TIMEOUT == ev)
//�� �������				{
//�� �������					if(bitOn || bitOff)
//�� �������					{
//�� �������						unsigned t = res ^ bitInv;
//�� �������						if((t & bitOn) || (bitOff & (t ^ bitOff))) 
//�� �������						{
//�� �������								OnceDo<typename Filt<List, Once>::Result>()();
//�� �������								return res;
//�� �������						}
//�� �������					}
//�� �������					if((((unsigned short *)&res)[1]) & sycle_ts)
//�� �������					{
//�� �������						dprint("ExceptionStopProc  %x\n", sycle_ts);
//�� �������						throw ExceptionStopProc();
//�� �������					}
//�� �������					DefaultDo<typename Filt<List, Proc>::Result>()();
//�� �������					if(GetTickCount() >= delay) throw ExceptionTimeOutProc();
//�� �������				}
//�� �������				else
//�� �������				{
//�� �������					arrEvents.Throw(ev - WAIT_OBJECT_0);
//�� �������				}
//�� �������			}
//�� �������		}
//�� �������	};

	
	template<class List>struct Test_OutBits
	{
		bool operator()()
		{
			unsigned bitOn = 0, bitOff = 0;
			SelectBits<typename Filt<List, On>::Result>()(bitOn);
			SelectBits<typename Filt<List, Off>::Result>()(bitOff);
			unsigned res = device1730.ReadOutput();
			return bitOn == (res & (bitOn | bitOff));
		}
	};

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
					return arrEvents.Throw(ev - WAIT_OBJECT_0);
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
			unsigned res = 0;
			BitsOut<typename Filt<List, On>::Result>()(res);

			device1730.Write(res);
		}
	};
///-----------------------------------------------------------------------------------
	template<>struct Off<i�ontrol�ircuits>
	{
		static unsigned &bit;
		static void Do(unsigned bits)
		{
		   if(0 == (bits & bit)) throw Exception�ontrol�ircuitsOffProc();
		}
	};
	
	template<>struct Off<iCycle>
	{
		static unsigned &bit;
		static void Do(unsigned bits)
		{
			 if(0 == (bits & bit)) throw Exception�ycleOffProc();
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
namespace
{
unsigned &Off<i�ontrol�ircuits>::bit = Singleton<InputBitTable>::Instance().items.get<i�ontrol�ircuits>().value;
unsigned &Off<iCycle>::bit =	Singleton<InputBitTable>::Instance().items.get<iCycle>().value;

#define AND_BITS(...) AND_Bits<TL::MkTlst<__VA_ARGS__>::Result>()
#define OR_BITS(...) OR_Bits<TL::MkTlst<__VA_ARGS__>::Result>()

#define OUT_BITS(...) OUT_Bits<TL::MkTlst<__VA_ARGS__>::Result>()()
#define SET_BITS(...) SET_Bits<TL::MkTlst<__VA_ARGS__>::Result>()()

#define TEST_OUT_BITS(...)Test_OutBits<TL::MkTlst<__VA_ARGS__>::Result>()()
	 bool &onTheJobCross = Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<Cross>>().value;
	 bool &onTheJobLong = Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<Long>>().value;
	 bool &onTheJobThickness = Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<Thickness>>().value;
	 bool &viewInterrupt = 	Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<ViewInterrupt>>().value;
}

void LogUSPCWindow_Open(void *)
{
	LogUSPCWindow::Open();
}

void Automat::Impl::Do()
{
	Log::Mess<LogMess::ProgramOpen>(0);
	LogMessageToTopLabel logMessageToTopLabel;
	AppKeyHandler::Init();
	try
	{
		while(true)
		{
			try
			{
Start:
				App::measurementOfRunning = false;	

				Sleep(500);

				AND_BITS(Ex<ExceptionRunProc>)(); //������ ������ ���������
				ResetEvent(App::ProgrammStopEvent);
				App::measurementOfRunning = true;
				Log::Mess<LogMess::WaitControlCircuitBitIn>();

				AND_BITS(Ex<ExceptionStopProc>, On<i�ontrol�ircuits>)(10000);
				Log::Mess<LogMess::PowerBMOn>();

				if(TEST_OUT_BITS(Off<oPowerBM>))
				{
					USPC::Close();
					OUT_BITS(On<oPowerBM>);
					Sleep(2000);
					if(!RestartService()) throw Exception_USPC_RestartService_ERROR_Proc();
					//��������� ��������� ��� �������������� ���� � ���������������(����� ��������)
					//��������� ��������� ��� �������� �����������
					if(!USPC::Open()) throw Exception_USPC_ERROR_Proc();
				}
				//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
				Log::Mess<LogMess::WaitCycle>();
				AND_BITS(Ex<ExceptionStopProc>, On<iCycle>, Proc<Off<i�ontrol�ircuits>>)(60 * 60 * 1000);	
				Log::Mess<LogMess::WaitReady>();
				AND_BITS(Ex<ExceptionStopProc>, On<iReady>, Proc<Off<i�ontrol�ircuits>>)(60 * 60 * 1000);	
				SET_BITS(On<oPowerBM>);
				//����������� �������������� ������� � ������
				//USPC::Start();
				//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
				OUT_BITS(On<oWork>);
				Log::Mess<LogMess::WaitControl>();
				USPC::Config();
				AND_BITS(Ex<ExceptionStopProc>, On<iControl>, Proc<Off<iCycle>>, Proc<Off<i�ontrol�ircuits>>)(25 * 1000);
				unsigned startTime = timeGetTime();
				//���� ������ � �������������� ��������
				Log::Mess<LogMess::InfoDataCollection>();
				USPC::Start();
				AND_BITS(Ex<ExceptionStopProc>, On<iBase>, Proc<Off<iCycle>>, Proc<Off<i�ontrol�ircuits>>, Proc<USPC_Do>)(60 * 60 * 1000);
				unsigned baseTime = timeGetTime();
				//��������� �������� ������� � ����� �� �����
				AutomatAdditional::ComputeSpeed(baseTime - startTime);
				dprint("baseTime %d    startTime %d  baseTime - startTime %d", baseTime, startTime, baseTime - startTime);
				//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
				AND_BITS(Ex<ExceptionStopProc>, Off<iControl>, Proc<Off<iCycle>>, Proc<Off<i�ontrol�ircuits>>, Proc<USPC_Do>)(60 * 60 * 1000);
				//���������� ����� USPC
				USPC::Stop();
				//������ ������, ����� �� �����
				unsigned stopTime = timeGetTime();
				compute.LengthTube(startTime, baseTime, stopTime);
				compute.Recalculation();
				
				//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
				Sleep(100);
				OUT_BITS(Off<oWork>);
				//����� ����������
			    if(viewInterrupt)
				{
					ResetEvent(App::ProgrammContinueEvent);
					ResetEvent(App::ProgrammRunEvent);
					if(compute.tubeResult)
					{
						Log::Mess<LogMess::ContineCycleOk>((double)compute.lengthTube / 1000);
					}
					else
					{
						Log::Mess<LogMess::ContineCycleBrak>((double)compute.lengthTube / 1000);
					}
					AppKeyHandler::Continue();
					//WaitForSingleObject(App::ProgrammContinueEvent, INFINITE);
					bool restart = 0 == AND_BITS(Ex<ExceptionRunProc>, Ex<ExceptionContinueProc>)(60 * 60 * 1000);
					SetEvent(App::ProgrammRunEvent);
					if(restart)goto Start;					
				}
				//todo � ����������� �� ����������� �������� ��������� ������� ���������1 � ���������2
				if(compute.tubeResult)
				{
					OUT_BITS(On<oResult1>);
					Log::Mess<LogMess::CycleOk>((double)compute.lengthTube / 1000);
				}
				else
				{
					OUT_BITS(On<oResult2>);
					Log::Mess<LogMess::CycleBrak>((double)compute.lengthTube / 1000);
				}
				Sleep(500);
				//��������� ������ ����������
				OUT_BITS(On<oToShiftThe>);
				//�������� ��������� �������� � ���� ������
				Stored::Do();
				//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			}
			catch(Exception�ontrol�ircuitsOffProc)
			{
				ResetEvent(App::ProgrammContinueEvent);
				Log::Mess<LogMess::AlarmControlCircuts>();
				device1730.Write(0);
			}	
			catch(Exception�ycleOffProc)
			{
				ResetEvent(App::ProgrammContinueEvent);
				Log::Mess<LogMess::AlarmCycle>();
				device1730.Write(0);
			}
			catch(ExceptionTimeOutProc)
			{
				ResetEvent(App::ProgrammContinueEvent);
				Log::Mess<LogMess::TimeoutPipe>();
				device1730.Write(0);
			}
			catch(ExceptionStopProc)
			{
				ResetEvent(App::ProgrammContinueEvent);
				Log::Mess<LogMess::InfoUserStop>();	
				device1730.Write(0);
				//	AppKeyHandler::Stop();
		//	ResetEvent(App::ProgrammContinueEvent);
			}
			catch(Exception_USPC_DO_ERROR_Proc)
			{
				ResetEvent(App::ProgrammContinueEvent);
				Log::Mess<LogMess::TimeoutPipe>();	
				device1730.Write(0);
			}
			catch(Exception_USPC_ERROR_Proc)
			{
				ResetEvent(App::ProgrammContinueEvent);
				Log::Mess<LogMess::AlarmUSPC>();
				USPC::Stop();
				device1730.Write(0);
				int ret = MessageBox(
					app.mainWindow.hWnd
					, L"������� ���� ��������� ���������?", L"������ ����� ������������� ��������"
					, MB_ICONEXCLAMATION | MB_YESNOCANCEL);
				if(IDYES == ret)
				{
					PostMessage(app.mainWindow.hWnd, WM_USER, (WPARAM)LogUSPCWindow_Open, 0);
				}
			}
			catch(Exception_USPC_RestartService_ERROR_Proc)
			{
				ResetEvent(App::ProgrammContinueEvent);
				Log::Mess<LogMess::AlarmRestartServiceError>();
				device1730.Write(0);
			}
			//USPC::Stop();
		//	AppKeyHandler::Stop();
		//	ResetEvent(App::ProgrammContinueEvent);
			dprint("DATA COMPLITE ******************************\n");
		}
	}
	catch(ExceptionExitProc)
	{
		CloseHandle(hThread);
		Log::Mess<LogMess::ProgramClosed>(0);
	}
	device1730.Write(0);
}

 Automat::Automat()
	: impl(Automat::Impl::Instance())
{}

void Automat::Init()
{
	impl.Init();
}

//--------------------------------------------------------------------------
//---------------------------------------------------------------------------
bool USPC_Test_BOOL = true;
DWORD WINAPI Test_USPC(LPVOID)
{
	try
	{				
		App::measurementOfRunning = true;

		Log::Mess<LogMess::PowerBMOn>();

		if(TEST_OUT_BITS(Off<oPowerBM>))
		{
			USPC::Close();
			OUT_BITS(On<oPowerBM>);
			Sleep(2000);
			if(!RestartService()) throw Exception_USPC_RestartService_ERROR_Proc();
			//��������� ��������� ��� �������������� ���� � ���������������(����� ��������)
			//��������� ��������� ��� �������� �����������
			if(!USPC::Open()) throw Exception_USPC_ERROR_Proc();
		}
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~				
		SET_BITS(On<oPowerBM>);
		//����������� �������������� ������� � ������
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		unsigned startTime = timeGetTime();
		//���� ������ � �������������� ��������
		Log::Mess<LogMess::InfoDataCollection>();
		USPC::Start();

		int test_counter = 0;				

		for(int i = 0; i < 200; ++i) 
		{
			switch(WaitForSingleObject(App::ProgrammStopEvent, 50))
			{
			case WAIT_TIMEOUT:
				{
					USPC_Do::Do(0);
					if(++test_counter >= 20)
					{
						dprint("tme %d",  timeGetTime() - startTime);
						test_counter = 0;
					}
				}
				break;
			case WAIT_OBJECT_0:
				{
					throw ExceptionStopProc();
				}
				break;
			}					
		}

		unsigned baseTime = timeGetTime();

		for(int i = 0; i < 100; ++i) 
		{
			switch(WaitForSingleObject(App::ProgrammStopEvent, 50))
			{
			case WAIT_TIMEOUT:
				{
					USPC_Do::Do(0);
					if(++test_counter >= 20)
					{
						dprint("tme %d",  timeGetTime() - startTime);
						test_counter = 0;
					}
				}
				break;
			case WAIT_OBJECT_0:
				{
					throw ExceptionStopProc();
				}
				break;
			}					
		}
		//��������� �������� ������� � ����� �� �����
		AutomatAdditional::ComputeSpeed(baseTime - startTime);
		dprint("baseTime %d    startTime %d  baseTime - startTime %d", baseTime, startTime, baseTime - startTime);
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		//���������� ����� USPC
		USPC::Stop();
		unsigned stopTime = timeGetTime();
		compute.LengthTube(startTime, baseTime, stopTime);
		compute.Recalculation();
		device1730.Write(0);
		AppKeyHandler::Stop();
	}
	catch(Exception�ontrol�ircuitsOffProc)
	{
		ResetEvent(App::ProgrammContinueEvent);
		Log::Mess<LogMess::AlarmControlCircuts>();
		device1730.Write(0);
	}	
	catch(Exception�ycleOffProc)
	{
		ResetEvent(App::ProgrammContinueEvent);
		Log::Mess<LogMess::AlarmCycle>();
		device1730.Write(0);
	}
	catch(ExceptionTimeOutProc)
	{
		ResetEvent(App::ProgrammContinueEvent);
		Log::Mess<LogMess::TimeoutPipe>();
		device1730.Write(0);
	}
	catch(ExceptionStopProc)
	{
		ResetEvent(App::ProgrammContinueEvent);
		Log::Mess<LogMess::InfoUserStop>();	
		device1730.Write(0);
	}
	catch(Exception_USPC_DO_ERROR_Proc)
	{
		ResetEvent(App::ProgrammContinueEvent);
		Log::Mess<LogMess::TimeoutPipe>();	
		device1730.Write(0);
	}
	catch(Exception_USPC_ERROR_Proc)
	{
		ResetEvent(App::ProgrammContinueEvent);
		Log::Mess<LogMess::AlarmUSPC>();
		USPC::Stop();
		device1730.Write(0);
		int ret = MessageBox(
			app.mainWindow.hWnd
			, L"������� ���� ��������� ���������?", L"������ ����� ������������� ��������"
			, MB_ICONEXCLAMATION | MB_YESNOCANCEL);
		if(IDYES == ret)
		{
			PostMessage(app.mainWindow.hWnd, WM_USER, (WPARAM)LogUSPCWindow_Open, 0);
		}
	}
	catch(Exception_USPC_RestartService_ERROR_Proc)
	{
		ResetEvent(App::ProgrammContinueEvent);
		Log::Mess<LogMess::AlarmRestartServiceError>();
		device1730.Write(0);
	}		
	catch(ExceptionExitProc)
	{
		Log::Mess<LogMess::ProgramClosed>(0);
	}
	App::measurementOfRunning = false;
	USPC_Test_BOOL = true;
	return 0;
}
//-------------------------------------------------------------------------
bool USPC_Test()
{
	if(USPC_Test_BOOL)
	{
		USPC_Test_BOOL = false;
		QueueUserWorkItem(Test_USPC, NULL, WT_EXECUTELONGFUNCTION);
		return true;
	}
	return false;
}

