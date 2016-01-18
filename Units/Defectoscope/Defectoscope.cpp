#include "stdafx.h"

#include <windows.h>
#include <commctrl.h>
#include "Base.hpp"
#include "App.h"
//#include "Window\MainWindow.h"
//#include "EmptyWindow.h"
//
//#include "DebugMess.h"
//#include "GlobalItems.h"
//#include "CommonTcp.h"
//#include "WindowsPosition.h"


#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

HINSTANCE hInstance;	
wchar_t *typeWindow = L"solid_group_150605";

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	CreateSemaphore(0, 0, 1, typeWindow);
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		HWND h = FindWindow(typeWindow, 0);
		SendMessage(h, WM_SYSCOMMAND, SC_RESTORE, 0);
		SetForegroundWindow(h);
		return 0;
	}
	::hInstance = hInstance;
	INITCOMMONCONTROLSEX iccx;
	iccx.dwSize=sizeof(INITCOMMONCONTROLSEX);
	iccx.dwICC=0;
	InitCommonControlsEx(&iccx);

	ULONG_PTR gdiplusToken; 
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;    
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
//--------------------------------------------------------------------
//D:\work\Buran9000\Common\Base\Base.hpp
//D:\work\Buran9000\Units\Defectoscope\Defectoscope.cpp
	Initialize initialize;
	
	App app;
	app.Init();

#ifdef THREAD_PRIORITY
	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
#endif

	MSG msg;  

	//	RegisterHotKey(NULL, 1, MOD_ALT | MOD_CONTROL | MOD_NOREPEAT, VK_RETURN); 

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);           
	}
	app.Destroy();
	Gdiplus::GdiplusShutdown(gdiplusToken);
	return (int) msg.wParam;
}
