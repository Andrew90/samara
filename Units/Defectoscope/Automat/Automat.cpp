#include "stdafx.h"
#include "Automat.h"
#include <MMSystem.h>
#include "App.h"
#include "AppBase.h"
#include "Device1730.h"
#include "LogMessages.h"
#include "LogBuffer.h"
#include "LogMessageToTopLabel.h"

struct Automat::Impl
{
	struct ExceptionExitProc{};
	struct ExceptionStopProc{};
	struct ExceptionTimeOutProc{};
	struct ExceptionContinueProc{};
	HANDLE hThread;
	Impl() {}
	static Automat::Impl &Instance(){static Automat::Impl x; return x;};
	void Do();
	static DWORD WINAPI ProcDo(LPVOID)
	{
		Automat::Impl::Instance().Do();
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
			 void operator()(O *, P *p)
			 {
				 p[TL::IndexOf<list, O>::value] = Ex<O>().handle;
			 }
		 };
		 template<class O, class P>struct ev
		 {
			 void operator()(O *, P *p)
			 {
				if(TL::IndexOf<list, O>::value == *p) throw O();
			 }
		 };
		 template<class P>struct ev<ExceptionContinueProc, P>
		 {
			 void operator()(ExceptionContinueProc *, P *){}
		 };
		 HANDLE h[TL::Length<list>::value];
		 ArrEvents()
		 {
			 TL::foreach<list, loc>()((TL::Factory<list> *)0, h);
		 }

		 void Throw(unsigned t)
		 {
			 TL::foreach<list, ev>()((TL::Factory<list> *)0, &t);
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

	template<class O, class P>struct __bits__
	{
		void operator()(O *o, P *p)
		{
			*p |= o->value;
		}
	};
	
	template<class List>struct SelectBits
	{
		void operator()(unsigned &bits)
		{
			bits = 0;
			TL::foreach<List, __bits__>()(&Singleton<InputBitTable>::Instance().items, &bits);
		}
	};
	template<>struct SelectBits<NullType>
	{
		void operator()(unsigned &bits)
		{
			bits = 0;
		}
	};

	template<class O, class P>struct __default_do__
	{
		void operator()(O *, P *)
		{
			O::Do();
		}
	};

	template<class List>struct DefaultDo
	{
		void operator()()
		{
			TL::foreach<List, __default_do__>()((Factory<List> *)0, (int *)0);
		}
	};
	template<>struct DefaultDo<NullType>
	{
		void operator()(){}
	};



	template<class List>struct OR_Bits
	{
		unsigned operator()(unsigned delay = (unsigned)-1)
		{
			if((unsigned)-1 != delay) delay += GetTickCount();
			unsigned bitOn, bitOff, bitInv;
			SelectBits<typename Filt<List, On>::Result>()(bitOn);
			SelectBits<typename Filt<List, Off>::Result>()(bitOff);
			SelectBits<typename Filt<List, Inv>::Result>()(bitInv);

			typedef TL::Append<typename Filt<List, Ex>::Result, ExceptionExitProc>::Result exeption_list;
			ArrEvents<exeption_list> arrEvents;
			
			while(true)
			{
				unsigned ev = WaitForMultipleObjects(dimention_of(arrEvents.h), arrEvents.h, FALSE, 5);
				if(WAIT_TIMEOUT == ev)
				{
					if(bitOn || bitOff)
					{
						unsigned res = device1730.Read();
						unsigned t = res ^ bitInv;
						if((t & bitOn) || (bitOff & (t ^ bitOff))) return res;
					}
					DefaultDo<typename Filt<List, Proc>::Result>()();
					if(GetTickCount() >= delay) throw ExceptionTimeOutProc();
				}
				else
				{
					arrEvents.Throw(ev - WAIT_OBJECT_0);
				}
			}
		}
	};

	template<class List>struct AND_Bits
	{
		unsigned operator()(unsigned delay = (unsigned)-1)
		{
			if((unsigned)-1 != delay) delay += GetTickCount();
			unsigned bitOn, bitOff, bitInv;
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
					if(bitOn)
					{						
						unsigned t = res ^ bitInv;
						if(bitOn == (t & (bitOn | bitOff))) return res;
					}
					DefaultDo<typename Filt<List, Proc>::Result>()();
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
};

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
				 AND_Bits<TL::MkTlst<
					 Ex<ExceptionContinueProc>
				 >::Result>()();

				  Log::Mess<LogMess::StartSycle>(0);

				 AND_Bits<TL::MkTlst<
					 On<SQ0>, On<SQ3>
				 >::Result>()(3000);

				  Log::Mess<LogMess::InfoOnWorkBitIn>(0);
				// Sleep(5000);

				  App::measurementOfRunning = true;//программа в цикле измерения
				  Log::Mess<LogMess::InfoDataCollectionComplete>(0);
				//  Sleep(1000);
				  AND_Bits<TL::MkTlst<
					 On<SQ0>, On<SQ3>
				 >::Result>()(5000);

				Log::Mess<LogMess::StopSycle>(0);
			 }
			 catch(ExceptionStopProc)
			 {
				 ResetEvent(App::ProgrammContinueEvent);
				 App::measurementOfRunning = false;	//программа вышла из цикла измерения
				 Log::Mess<LogMess::InfoUserStop>(0);
			 }
			 catch(ExceptionTimeOutProc)
			 {
				 ResetEvent(App::ProgrammContinueEvent);
				 App::measurementOfRunning = false;	//программа вышла из цикла измерения
				 Log::Mess<LogMess::TimeoutPipe>(0);
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