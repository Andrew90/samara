#include "stdafx.h"
#include "SelectTypeSizeList.h"
#include <WindowsX.h>
#include "AppBase.h"
#include "App.h"
#include "message.h"
#include "MainWindow.h"

namespace
{

	template<class T, class D>struct __sel__
	{
		bool operator()(T &t, D &d)
		{
			ComboBox_AddString(d, t.items.get<NameParam>().value);
			return false;
		}
	};

	void FillSelect(HWND hWnd)
	{
		CBase base(ParametersBase().name());
		if(base.IsOpen())
		{
			Select<ParametersTable>(base).ExecuteLoop<__sel__>(hWnd);
			int num = ComboBox_FindStringExact(hWnd, 0, Singleton<ParametersTable>::Instance().items.get<NameParam>().value);
			if(CB_ERR != num) ComboBox_SetCurSel(hWnd, num);
		}
	}
}

extern HINSTANCE hInstance;

SelectTypeSizeList::SelectTypeSizeList() : obj(NULL), ptr(NULL){}

void SelectTypeSizeList::Create(HWND h)
{
	hWnd = CreateWindow(L"COMBOBOX", NULL
		, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST|CBS_AUTOHSCROLL |WS_VSCROLL 
		, 0, 0, 0, 0, h, 0, hInstance, 0
		);
	SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG)this);//&SelectTypeSizeList::Do);
	HFONT font = CreateFont(40,0,0,90,900,TRUE,0,0,ANSI_CHARSET,0,0,0,0, L"Times New Roman"); 
	SendMessage(hWnd, WM_SETFONT, (WPARAM)font, (LPARAM)0);
	FillSelect(hWnd);
}

void SelectTypeSizeList::Size(int x, int y, int width)
{	
	MoveWindow(hWnd, x, y, width, 525, TRUE);
}

void SelectTypeSizeList::Do(TCommand &m)
{
	if(1 == m.isAcselerator)
	{
		wchar_t buf[128];
		GetWindowText(m.hControl, buf, dimention_of(buf));
		CBase base(ParametersBase().name());
		if(base.IsOpen())
		{			
			NameParam::type_value name;
			name = buf;
			ParametersTable typeSizeParam;
			int id = Select<ParametersTable>(base).eq<NameParam>(name).Execute(typeSizeParam);
			if(id != 0)
			{
				CurrentParametersTable t;
				t.items.get<CurrentID>().value = id;
				UpdateWhere<CurrentParametersTable>(t, base).ID(1).Execute();

				AppBase::InitTypeSizeTables(base);
				//SelectTypeSizeList &o = ((MainWindow *)GetWindowLongPtr(m.hwnd, GWLP_USERDATA))->select;
				//if(o.obj&&o.ptr) (o.obj->*o.ptr)();
				if(obj&&ptr)(obj->*ptr)();

				SetFocus(m.hwnd);
			}
		}
	}
}

void SelectTypeSizeList::AddMenuItem(wchar_t *buf)
{
	ComboBox_AddString(hWnd, buf);
	int num = ComboBox_FindStringExact(hWnd, 0, buf);
	if(CB_ERR != num) ComboBox_SetCurSel(hWnd, num);
}

void SelectTypeSizeList::DelMenuItem(wchar_t *buf)
{
	int num = ComboBox_FindStringExact(hWnd, 0, buf);
	if(CB_ERR != num)
	{
		ComboBox_DeleteString(hWnd, num);
		ComboBox_SetCurSel(hWnd, 0);
		ComboBox_GetText(hWnd, buf, 128);
	}
	else
	{
		buf[0] = 0;
	}
}