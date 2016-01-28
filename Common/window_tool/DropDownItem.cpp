#include "stdafx.h"
#include "DropDownItem.h"
#include "message.h"
#include "typelist.hpp"

DropDown::DropDown()
	: ptr(NULL)
	, obj(NULL)
{}

void DropDown::Do(TCommand &m)
{
	if(1 == m.isAcselerator)
	{
		wchar_t buf[128];
		GetWindowText(m.hControl, buf, dimention_of(buf));
		if(obj&&ptr) (obj->*ptr)(buf);
	}
}

void DropDown::Create(HWND h, int fontSize, int height, const wchar_t *textItems, int countTextItems)
{
}