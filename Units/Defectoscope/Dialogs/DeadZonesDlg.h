#include "stdafx.h"
#include "Dialogs.h"

namespace
{
}	   qwkjksklasklasasnj

void DeadZonesDlg::Do(HWND h)
{
	t.items.get<NameParam>().value = Singleton<NameParam>::Instance().value;
	if(TemplDialog<DeadAreaTable>(DeadAreaTable()).Do(h, L"Удалить типоразмер")
	   )
	{
	}
}