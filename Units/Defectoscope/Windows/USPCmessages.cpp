#include "stdafx.h"
#include "USPCmessages.h"
//#include "USPCChartViewer.h"
//#include "USPCBottomPanel.h"

#include "USPCWindow.h"

namespace
{
	template<class List, template<class>class W>struct Filtr;
	template<class Head, class Tail, template<class>class W>struct Filtr<Tlst<Head, Tail>, W>
	{
		typedef typename Filtr<Tail, W>::Result Result;
	};
	template<class Head, class Tail, template<class>class W>struct Filtr<Tlst<W<Head>, Tail>, W>
	{
		typedef Tlst<W<Head>, typename Filtr<Tail, W>::Result> Result;
	};
	template<template<class>class W>struct Filtr<NullType, W>
	{
		typedef NullType Result;
	};

	template<class O, class P>struct __set_gate_item_param__
	{
		void operator()(O *o, P *p)
		{
			dprint("^^^^^^  %s\n", typeid(O).name());
			typedef typename USCPpageItems<typename TL::Inner<O>::Result> T;
			T &t = p->get<T>();
			dprint("^^^^^^  %s\n", typeid(t.).name());
		}
	};
}

/*
void SetDateToGateItems(USPCChartViewer &viewer, USPCBottomPanel &panel)
{
	typedef Filtr<USPCChartViewer::TChart::items_list, GateItem>::Result list;
	dprint("\n~~~~ %s\n", typeid(list).name());
}
*/

void SetParamToGateItem()
{
	typedef Filtr<USPCChartViewer::TChart::items_list, GateItem>::Result list;
	dprint("\n~~~~ %s\n", typeid(list).name());

	TL::foreach<list, __set_gate_item_param__>()(
		&USPCWindow::Instance().uspcChartViewer.chart.items
		, &USPCWindow::Instance().panel.uspcTabs.items
		);
}