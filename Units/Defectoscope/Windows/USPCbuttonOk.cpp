#include "stdafx.h"
#include "USPCbuttonOk.h"
#include <Windows.h>
#include "DebugMess.h"

extern HINSTANCE hInstance;

USPCbuttonOk::USPCbuttonOk()
	: obj(NULL)
	, ptr(NULL)
{}

void USPCbuttonOk::Do(TCommand &l)
{
	zprint(" ~~~");
	if(obj&&ptr) (obj->*ptr)();
}

void USPCbuttonOk::Create(HWND h)
{
	hWnd = CreateWindow(L"button", L"Применить"
				, WS_VISIBLE | WS_CHILD | WS_TABSTOP
				, 0, 0, 0, 0, h, NULL, hInstance, NULL
				);
	SetWindowLongPtr(hWnd, GWLP_USERDATA, (DWORD)this);
}

void USPCbuttonOk::Size(int x, int y)
{
	  MoveWindow(hWnd, x, y, 200, 25, TRUE);
}
