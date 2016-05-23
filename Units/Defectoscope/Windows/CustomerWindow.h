#pragma once
#include "GridWindow.h"
#include "Grid.h"
#include "CustomerData.h"
#include "MessagesInterface.h"

class CustomerWindow
{
public:
	class OkBtn: public TEvent
	{
	public:
		CustomerWindow &owner;
		OkBtn(CustomerWindow &);
		void Do(TCommand &);
	} okBtn;
	class CancelBtn: public TEvent
	{
	public:
		CustomerWindow &owner;
		CancelBtn(CustomerWindow &);
		void Do(TCommand &);
	} cancelBtn;
private:
	GridWindow gridViewer;
public:
	GridNotify grid;
public:
	CustomerData data;
public:
	HWND hWnd, hOk, hCancel, hResult;
	wchar_t result[128];
	CustomerWindow();
	void operator()(TSize &);
	void operator()(TCommand &);
	unsigned operator()(TCreate &);
	void operator()(TDestroy &);
	unsigned operator()(TNotify &);

	void RClick(int, wchar_t *);
	void LClick(int, wchar_t *);
	static wchar_t *Title();
};