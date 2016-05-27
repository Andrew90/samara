// This is the main DLL file.

#include "stdafx.h"

#include "DLL.h"
namespace DLL {


	void Wapper::Create()
	{
		if(pimpl != 0) 	pimpl = (void *)GridChart::Create();
	}
	void Wapper::Destroy(){}
void Wapper::Size(HWND h, unsigned msg, unsigned resizing, unsigned short width, unsigned short height)
{
}

void Wapper::Paint(HWND h, unsigned msg, HDC hdc)
{
}
}
