#include "stdafx.h"
#include "USPCBottomPanel.h"
#include "TabControlUnits.h"
#include "AppBase.h"
/*
PARAM_TITLE(USPCBottomPanel::USPCTabs::gateIFPage, L"gateIF")
PARAM_TITLE(USPCBottomPanel::USPCTabs::gate1Page, L"gate1")
PARAM_TITLE(USPCBottomPanel::USPCTabs::gate2Page, L"gate2")
PARAM_TITLE(USPCBottomPanel::USPCTabs::scopePage, L"scope")
*/
PARAM_TITLE(USCPpageItems<gateIF>, L"gateIF")
PARAM_TITLE(USCPpageItems<gate1>, L"gate1")
PARAM_TITLE(USCPpageItems<gate2>, L"gate2")
PARAM_TITLE(USCPpageItems<scope>, L"scope")

PARAM_TITLE(position, L"position")
PARAM_TITLE(width, L"width")
PARAM_TITLE(level, L"level")
PARAM_TITLE(nb_alarm_level, L"alarm level")

PARAM_TITLE(offset, L"offset")
PARAM_TITLE(range, L"range")
PARAM_TITLE(velocity, L"velocity")

USPCBottomPanel::USPCBottomPanel(int &u, int &s)
: unit(u)
, sens(s)
, uspcTabs(*this)
, tabControl(uspcTabs)
{
}

unsigned USPCBottomPanel::operator()(TCreate &l)
{	
    tabControl.x = 0;
	tabControl.y = 0;
	tabControl.width = 500;	
	tabControl.Create(l.hwnd);
	uspcButtonOk.Create(l.hwnd);
	firstSize = true;
	return 0;
}

void USPCBottomPanel::operator()(TNotify &l)
{
	tabControl.Notify(l);
}

void USPCBottomPanel::operator()(TCommand &l)
{
	zprint(" ~~~");
}

void USPCBottomPanel::operator()(TSize &l)
{
	RECT r;
	GetClientRect(l.hwnd, &r);
	tabControl.height = r.bottom;
	if(firstSize)
	{
		firstSize = false;
		tabControl.Show(true);
	}
	tabControl.Size(l);
	uspcButtonOk.Size(tabControl.width + 20, tabControl.height - 40);
}
