#pragma once
#include "LogMessages.h"

class Log
{	
	friend class Initialization;
	struct Inner;
	Inner &inner;
	Log();
	Log &operator =(const Log &);
	Log(Inner &);
	void Insert(int id, double val, void (*Do)(LONG , LONG &));
	template<class T>struct the_index_variable_is_out_of_range_look_at_N{typedef T Result;};
	template<>struct the_index_variable_is_out_of_range_look_at_N<void>{typedef int Result;};
	static Log &hidden;
	template<class T>static double Convert(T t){return (double)t;}
	static double Convert(const char *t)
	{
		double d = 0;
		*(unsigned *)&d = (unsigned)t; 
		return d;
	}
public:	
	template<int N>struct Filter
	{
		static void Do(LONG currentID, LONG &lastId){InterlockedExchange(&lastId, currentID);}
	};
	struct TData
	{
		unsigned id;
		unsigned time;
		double value;
	};
#pragma warning(disable: 4101)
	static LONG lastMessage;
	template<int N>static void Mess(typename the_index_variable_is_out_of_range_look_at_N<typename LogMess::Row<N>::type>::Result value)
	{
		hidden.Insert(N, Convert(value), Filter<N>::Do);	
	}
#pragma warning(default: 4101)
	static bool IsRow(int , TData *&);
	static int LastMessageIndex();
	static bool LastMessage(TData *&d);
	static void Start();
};

#define DROP_MESSAGE(idMess)template<>void Log::Filter<idMess>::Do(LONG , LONG &){}
DROP_MESSAGE(LogMess::ErrStop)
#undef DROP_MESSAGE
