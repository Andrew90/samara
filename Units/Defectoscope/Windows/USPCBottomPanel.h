#pragma once
#include "message.h"
#include "TabControlUnits.h"
#include "USPCbuttonOk.h"
#include "USPCParam.h"
#include "App.h"
#include "USPCpageItems.hpp"
#include "DebugMess.h"

class USPCBottomPanel
{
	typedef TL::MkTlst<
		 LongParam
		 , CrossParam
		 , ThicknessParam
	 >::Result items_list;
	bool firstSize;
public:
	int &unit;
	int &sens;
public:
	
	struct USPCTabs
	{
		/*
		struct gateIFPage
		{
			int &unit;
			int &sens;
			typedef gateIF::items_list items_list;
			TL::Factory<items_list> items;
			template<class T>gateIFPage(T &t): unit(t.unit), sens(t.sens){}
		};
		struct gate1Page
		{
			int &unit;
			int &sens;
			typedef gate1::items_list items_list;
			TL::Factory<items_list> items;
			template<class T>gate1Page(T &t): unit(t.unit), sens(t.sens){}
		};
		struct gate2Page
		{
			int &unit;
			int &sens;
			typedef gate2::items_list items_list;
			TL::Factory<items_list> items;
			template<class T>gate2Page(T &t): unit(t.unit), sens(t.sens){}
		};
		struct scopePage
		{
			int &unit;
			int &sens;
		   typedef scope::items_list items_list;
		   TL::Factory<items_list> items;
		   template<class T>scopePage(T &t): unit(t.unit), sens(t.sens){}
		};
		typedef TL::MkTlst<
			 gateIFPage
			, gate1Page
			, gate2Page
			, scopePage
		>::Result items_list;
		*/

		typedef TL::MkTlst<
			USCPpageItems<gateIF>
			, USCPpageItems<gate1>
			, USCPpageItems<gate2>
			, USCPpageItems<scope>
		>::Result items_list;
		
		TL::Factory<items_list> items;
		template<class T>USPCTabs(T &t): items(t){}
	};
	TL::Factory<items_list> items;
	USPCTabs uspcTabs;
	TabControlUnit<USPCTabs> tabControl;
	USPCbuttonOk uspcButtonOk;	
public:
	HWND hWnd;
	USPCBottomPanel(int &, int &);
	unsigned operator()(TCreate &);
	void operator()(TNotify &);
	void operator()(TCommand &);
	void operator()(TSize &);
};