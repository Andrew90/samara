#pragma once
#include "MessagesInterface.h"
class DropDown: public TCommandEvent
{
	struct TObj{};
	void (TObj::*ptr)(wchar_t *);
	TObj *obj;
public:
	HWND hWnd;
	DropDown();
	void Create(HWND, int fontSize, int height, const wchar_t *textItems, int countTextItems);
	void Do(TCommand &);
	template<class T>void SetCommandHandler(T *t, void (TObj::*t_ptr)(wchar_t *))
	{
		obj = (TObj *)t;
		ptr = (void (TObj::*)(wchar_t *))t_ptr;
	}
};