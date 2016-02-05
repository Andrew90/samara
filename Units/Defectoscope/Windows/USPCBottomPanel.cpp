#include "stdafx.h"
#include "USPCBottomPanel.h"
#include "TabControlUnits.h"
#include "AppBase.h"
#include "MenuAPI.h"
#include "USPCmessages.h"

PARAM_TITLE(USCPpageItems<gateIF>, L"gateIF")
PARAM_TITLE(USCPpageItems<gate1>, L"gate1")
PARAM_TITLE(USCPpageItems<gate2>, L"gate2")
PARAM_TITLE(USCPpageItems<scope>, L"scope")



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
	uspcButtonOk.Create(l.hwnd, L"Применить");
	firstSize = true;
	uspcButtonOk.SetCommandHandler(&tabControl, &TabControlUnit<USPCTabs>::StorePage);
	return 0;
}

void USPCBottomPanel::operator()(TNotify &l)
{
	EventDo(l);
}

void USPCBottomPanel::operator()(TCommand &l)
{
	EventDo(l);
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

void USPCBottomPanel::UpdatePage(int)
{
	tabControl.UpdatePage();
	SetParamToGateItem();
}

