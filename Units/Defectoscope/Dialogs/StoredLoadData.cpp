#include "stdafx.h"
#include "Dialogs.h"
#include "DlgFiles.h"
#include "Stored.h"
#include "Compute.h"
#include "AnimationControl.h"

namespace
{
	wchar_t path[512];
	struct __load__
	{		
		__load__(wchar_t *path_)
		{
			wcscpy(path, path_);
			QueueUserWorkItem(ThreadProc, NULL, WT_EXECUTELONGFUNCTION);
		}
		static DWORD WINAPI ThreadProc(_In_ LPVOID lpParameter)
		{
			Stored::DataFromFile(path);		
			compute.Recalculation();
			AnimationWindow::Destroy();
			return 0;
		}
	};
	struct __store__
	{		
		__store__(wchar_t *path_)
		{
			wcscpy(path, path_);
			QueueUserWorkItem(ThreadProc, NULL, WT_EXECUTELONGFUNCTION);
		}
		static DWORD WINAPI ThreadProc(_In_ LPVOID lpParameter)
		{
			Stored::DataToFile(path);
			AnimationWindow::Destroy();
			return 0;
		}
	};
}

void StoredDataDlg::Do(HWND h)
{
	SaveData o(h, L"���������");
	if(o())
	{
		AnimationWindow::Init(h, L"����������");
		__store__(o.sFile);
		return;
	}
	MessageBox(h, L"���� �� �������", L"������!!!", MB_ICONERROR);
}

void LoadDataDlg::Do(HWND h)
{
	OpenData o(h, L"���������");
	if(o())
	{
		AnimationWindow::Init(h, L"��������");
		__load__(o.sFile);
		return;
	}
	MessageBox(h, L"���� �� ��������", L"������!!!", MB_ICONERROR);
}