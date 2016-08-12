#pragma once
#include "message.h"
#include "ColorLabel.h"

class TopLabelViewer
{
public:
	Gdiplus::Bitmap *backScreen;
public:
	ColorLabel label;
public:
	HWND hWnd;
	TopLabelViewer();
	unsigned operator()(TSize &);
	unsigned operator()(TPaint &);
	unsigned operator()(TUser &);
	void SetMessage(wchar_t *);
	void operator()(TDestroy &);
};

