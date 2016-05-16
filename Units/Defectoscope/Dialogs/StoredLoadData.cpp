#include "stdafx.h"
#include "Dialogs.h"
#include "DlgFiles.h"
#include "Stored.h"
#include "Compute.h"

void StoredDataDlg::Do(HWND h)
{
	SaveData o(h, L"Сохранить");
	if(o())
	{
		Stored::DataToFile(o.sFile);
		return;
	}
	MessageBox(h, L"Файл не сохранён", L"Ошибка!!!", MB_ICONERROR);
}

void LoadDataDlg::Do(HWND h)
{
	OpenData o(h, L"Загрузить");
	if(o())
	{
		Stored::DataFromFile(o.sFile);
		compute.Recalculation();
		return;
	}
	MessageBox(h, L"Файл не загружен", L"Ошибка!!!", MB_ICONERROR);
}