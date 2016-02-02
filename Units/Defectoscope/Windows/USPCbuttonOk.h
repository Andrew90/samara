#pragma once
#include "MessagesInterface.h"

class USPCbuttonOk : public TCommandEvent
{
	class TObj{} *obj;
	void(TObj::*ptr)();
	HWND hWnd;
public:
	USPCbuttonOk();
	void Do(TCommand &);
	void Create(HWND);
	void Size(int, int);
	template<class T>void Set(T *o, void(TObj::*p)())
	{
		obj = (TObj *)o;
		ptr = (void(TObj::*)())p;
	}
};