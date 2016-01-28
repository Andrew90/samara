#include "stdafx.h"
#include <CommCtrl.h>
#include "MenuAPI.h"
#include "typelist.hpp"
#include "MessagesInterface.h"
//---------------------------------------------------------------------------
void GetMenuToolBarEvent(TCommand &m)
{
	if(m.hControl)
	{
		if(m.id)
		{
			TBBUTTONINFO button_info;
			button_info.cbSize = sizeof(button_info);
			button_info.dwMask = TBIF_LPARAM;
			SendMessage(m.hControl, (UINT) TB_GETBUTTONINFO, m.id , (LPARAM)&button_info);
			((void (__cdecl *)(HWND))(button_info.lParam))(m.hwnd);
		}
		else if(TCommandEvent *p = (TCommandEvent *)GetWindowLongPtr(m.hControl, GWLP_USERDATA))
		{
			//((void (*)(TCommand &))(p))(m);
			p->Do(m);
		}
	}
	else
	{
		MENUITEMINFO mii;
		mii.cbSize = sizeof(MENUITEMINFO);
		mii.fMask = MIIM_DATA;
		if(GetMenuItemInfo(GetMenu(m.hwnd), m.id, false, &mii))
		{
			((void (__cdecl *)(HWND))(mii.dwItemData))(m.hwnd);
		}
	}
}