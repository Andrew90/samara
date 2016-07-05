#include "stdafx.h"
#include "USPC.h"
#include "USPCData.h"
#include "AppBase.h"
#include "ut_files.h"
#include "DebugMess.h"		
#include "Config.h"
#ifndef DEBUG_ITEMS
#pragma comment(lib, "..\\..\\Common\\USPC\\USPC7100.lib")
#else
ULONG WINAPI USPC7100_Acq_Start
    (
    INT Board,                              // [in]
    INT NumberOfScansToAcquire              // [in]
	){return 1;}
ULONG WINAPI USPC7100_Open
    (
    INT boot                                // [in]
	){return 1;}
ULONG WINAPI USPC7100_Load
    (
    INT Board,                              // [in]
    INT Test,                               // [in]
    LPCSTR file                             // [in]
	){return 1;}
ULONG WINAPI USPC7100_Close(void){return 1;}
ULONG WINAPI USPC7100_Acq_Stop
    (
    INT Board                               // [in]
	){return 1;}
ULONG WINAPI USPC7100_Acq_Get_Status
    (
    INT Board,                              // [in]
    ULONG *Status,                          // [out]
    ULONG *NumberOfScansAcquired,           // [out]
    ULONG *NumberOfScansRead,               // [out]
    ULONG *BufferSize,                      // [out]
    ULONG *BlocSize                         // [out]
	){return 1;}
ULONG WINAPI USPC7100_Acq_Read
    (
    INT Board,                              // [in]
    INT NumberOfScansToRead,                // [in]
    INT TimeOut,                            // [in]
    ULONG *NumberRead,                      // [out]
    ULONG *ScansBacklog,                    // [out]
    UCHAR *pData                            // [out]
	){return 1;}
#endif

namespace USPC
{
	wchar_t lastTypeSize[128];
	wchar_t (&currentTypeSize)[128] = Singleton<ParametersTable>::Instance().items.get<NameParam>().value.buffer;
	ItemData<Long> &longData = Singleton<ItemData<Long> >::Instance();
	ItemData<Cross> &clossData = Singleton<ItemData<Cross> >::Instance();
	ItemData<Thickness> &thickData = Singleton<ItemData<Thickness> >::Instance();

	bool open = false;

	typedef TL::MkTlst<Thickness, Long, Cross>::Result items_list;

	template<class T>struct __board__;
	template<>struct __board__<Thickness>
	{
		static const int value = App::thickness_number;
	};
	template<>struct __board__<Long>
	{
		static const int value = App::long_number;
	};
	template<>struct __board__<Cross>
	{
		static const int value = App::cross_number;
	};

	template<class O, class P>struct __start__
	{
		bool operator()(P *p)
		{
			if(Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<O>>().value)
			{
				Singleton<ItemData<O> >::Instance().Start();
				unsigned err = USPC7100_Acq_Start(__board__<O>::value, -1);
				*p = err;
				return 0 == err;
			}
			return true;
		}
	};

	struct __test_data__
	{
		ULONG status, err;
		int board;
		__test_data__(): status(0), err(0), board(-1){}
	};

	template<class O, class P>struct __test__
	{
		bool operator()(P *p)
		{
			if(Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<O>>().value)
			{
				Singleton<ItemData<O> >::Instance().Start();
				ULONG numberOfScansAcquired, numberOfScansRead, bufferSize, scanSize;
				p->err =  USPC7100_Acq_Get_Status(__board__<O>::value, &p->status, &numberOfScansAcquired, &numberOfScansRead, &bufferSize, &scanSize);
				p->board = __board__<O>::value;
				return 0 == p->err;
			}
			return true;
		}
	};

	template<class O, class P>struct __stop__
	{
		void operator()()
		{
			USPC7100_Acq_Stop(__board__<O>::value);
		}
	};

	template<class O, class P>struct __do__
	{
		bool operator()(P *p)
		{
			if(Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<O>>().value)
			{
				ItemData<O> &item = Singleton<ItemData<O> >::Instance();
				ULONG numberRead;
				ULONG scansBacklog;
				UCHAR *pData = item.CurrentFrame();
				unsigned err = USPC7100_Acq_Read(
					__board__<O>::value
					, -1
					, 0
					, &numberRead
					, &scansBacklog
					, pData
					);
				if(numberRead > 0)
				{
					item.OffsetCounter(numberRead);
				}
				*p = err;
				return 0 == err;
			}
			return true;
		}
	};

	bool Open()
	{
		bool res = false;
		unsigned err = 0;
		wchar_t path[256];
		if(wcscmp(currentTypeSize, lastTypeSize))
		{
			res = ExistCurrentUSPCFile(path);
			if(res)wcscpy(lastTypeSize, currentTypeSize);
		}
		else
		{
			res = true;
		}
		if(res)
		{
			err = USPC7100_Open(2);
			dprint("USPC7100_Open err %x\n", err);
			if(!err)
			{
				char b[1024];
				wcstombs(b, path, wcslen(path) + 1); 
				err = USPC7100_Load(-1, -1, b);
				dprint("USPC7100_Load err %x\n", err);
			}
			res = 0 == err;
		}
		if(res)
		{
			__test_data__ data;
			res = TL::find<items_list, __test__>()(&data);
			if(!res)
			{
				dprint("test board %d  status %d err %d", data.board, data.status, data.err);
			}
		}
		return open = res;
	}

	void Close()
	{
		if(open)
		{
			unsigned err = USPC7100_Close();
			dprint("USPC7100_Close err %x\n", err);
			open = false;
		}
	}

	bool Start()
	{
		unsigned err = 0;
		bool b = TL::find<items_list, __start__>()(&err);
		if(!b)
		{
			TL::foreach<items_list, __stop__>()();
		}
		dprint("start err %x\n", err);
		return b;
	}

	void Stop()
	{
		TL::foreach<items_list, __stop__>()();
	}

	bool Do()
	{
		int err = 0;
		bool b = TL::find<items_list, __do__>()(&err);
		if(!b)
		{
			dprint("do err %x\n", err);
		}
		return b;
	}
}