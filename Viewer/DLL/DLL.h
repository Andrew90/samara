// DLL.h

#pragma once
#include "Lib.h"

using namespace System;

namespace DLL {

	public ref class Wapper
	{	
		void *pimpl;
	public:	
		void Create();
		void Destroy();
		void Size(HWND h, unsigned msg, unsigned resizing, unsigned short width, unsigned short height);
		void Paint(HWND h, unsigned msg, HDC hdc);
	};
}
