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
			//dprint("^^^^^^  %s\n", typeid(O).name());
			typedef typename TL::Inner<O>::Result Inner;
			typedef typename USCPpageItems<Inner> T;
			T &t = p->get<T>();
			//dprint("tttt  %f\n", t.get<position<typename TL::Inner<O>::Result> >().value);
			o->x     = t.get<position<Inner> >().value;
			o->y     = t.get<level   <Inner> >().value;
			o->width = t.get<width   <Inner> >().value;
		}
	};
}

void SetParamToGateItem()
{
	typedef Filtr<USPCChartViewer::TChart::items_list, GateItem>::Result list;
	USPCWindow &w = USPCWindow::Instance();

	TL::foreach<list, __set_gate_item_param__>()(
		&w.uspcChartViewer.chart.items
		, &w.panel.uspcTabs.items
		);

	w.uspcChartViewer.Update();
}