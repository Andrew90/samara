#pragma once
#include "ThresholdWindow.h"

class LongThresholdWindow: public TresholdWindow
{
public:
	typedef TresholdWindow Parent;
	unsigned operator()(TCreate &);
	void operator()(TClose &);
	static wchar_t *Title();
};