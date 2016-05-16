#include "stdafx.h"
#include "Dialogs.h"
#include "DlgFiles.h"
#include "Stored.h"
#include "Compute.h"

void StoredDataDlg::Do(HWND h)
{
	SaveData o(h, L"���������");
	if(o())
	{
		Stored::DataToFile(o.sFile);
		return;
	}
	MessageBox(h, L"���� �� �������", L"������!!!", MB_ICONERROR);
}

void LoadDataDlg::Do(HWND h)
{
	OpenData o(h, L"���������");
	if(o())
	{
		Stored::DataFromFile(o.sFile);
		compute.Recalculation();
		return;
	}
	MessageBox(h, L"���� �� ��������", L"������!!!", MB_ICONERROR);
}