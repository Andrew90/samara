#pragma once
#include "GridWindow.h"
#include "Grid.h"
//#include "OperatorData.h"
#include "MessagesInterface.h"
#include "StoredBase.h"
#include "PacketTemplateData.hpp"


class OperatorWindow
{
public:
	class OkBtn: public TEvent
	{
	public:
		OperatorWindow &owner;
		OkBtn(OperatorWindow &);
		void Do(TCommand &);
	} okBtn;
	class CancelBtn: public TEvent
	{
	public:
		OperatorWindow &owner;
		CancelBtn(OperatorWindow &);
		void Do(TCommand &);
	} cancelBtn;
private:
	GridWindow gridViewer;
public:
	GridNotify grid;
public:
	PacketTemplateData<OperatorsTable, Operator> data;
public:
	HWND hWnd, hOk, hCancel, hResult;
	wchar_t result[128];
	OperatorWindow();
	void operator()(TSize &);
	void operator()(TCommand &);
	unsigned operator()(TCreate &);
	void operator()(TDestroy &);
	unsigned operator()(TNotify &);

	void RClick(int, wchar_t *);
	void LClick(int, wchar_t *);
	static wchar_t *Title();
};