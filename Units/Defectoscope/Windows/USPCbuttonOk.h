#pragma once
#include "MessagesInterface.h"

class USPCbuttonOk : public TEvent
{
	class TObj{} *obj;
	void(TObj::*ptr)();
	HWND hWnd;
public:
	USPCbuttonOk();
	void Do(TCommand &);
	void Create(HWND);
	void Size(int, int);
	template<class T>void SetCommandHandler(T *o, void(T::*p)())
	{
		obj = (TObj *)o;
		ptr = (void(TObj::*)())p;
	}
};