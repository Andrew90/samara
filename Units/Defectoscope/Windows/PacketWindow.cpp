#include "stdafx.h"
#include "PacketWindow.h"
#include "typelist.hpp"
#include "StoredBase.h"
#include "ParamDlg.h"
#include "PacketBase.h"
#include "Common.h"
#include "PacketTemplateWindow.hpp"

extern HINSTANCE hInstance;

struct PacketWindow
{
	static const int ID_Operator = 1;
	static const int ID_Customer = 2;
	static const int ID_OK = 3;
	static const int ID_CANCEL = 4;

	HWND hNumber, hEditOp, hEditCs, hOk, hCancel;
	static LRESULT CALLBACK Proc(HWND h, UINT msg, WPARAM wParam, LPARAM lParam);
	bool Do(HWND );
	bool OkBtn();
};

bool PacketWindowDlg(HWND h)
{
	PacketBase::Restore(
		Singleton<Operator>::Instance().value
		, Singleton<Customer>::Instance().value
		, Singleton<NumberPacket>::Instance().value
		);
	return PacketWindow().Do(h);
};

PARAM_TITLE(Operator, L"Оператор")
CHECK_EMPTY_STRING(Operator)

PARAM_TITLE(Customer, L"Заказчик")
CHECK_EMPTY_STRING(Customer)

LRESULT CALLBACK PacketWindow::Proc(HWND h, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_COMMAND:
		{
			PacketWindow *e = (PacketWindow *)GetWindowLong(h, GWL_USERDATA);	
			switch(wParam)
			{
			case ID_OK:
				if(e->OkBtn())EndDialog(h, TRUE);
				return TRUE;
			case ID_CANCEL:
				EndDialog(h, FALSE);
				return TRUE;
			case ID_Operator:
				{
					typedef PacketTemplateWindow<OperatorsTable, Operator> OperatorWindow;
					HWND hh = Common::OpenWindowBackGround<OperatorWindow>::Do(h);
					OperatorWindow *x = (OperatorWindow *)GetWindowLongPtr(hh, GWLP_USERDATA);
					x->hResult = e->hEditOp;
					SetWindowPos(hh, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
					DWORD dwStyle = GetWindowLong(hh, GWL_STYLE);
					dwStyle &= ~(WS_MAXIMIZEBOX | WS_MINIMIZEBOX);
					SetWindowLong(hh, GWL_STYLE, dwStyle);
				}
				return TRUE;
			case ID_Customer:
				{
					typedef PacketTemplateWindow<CustomersTable, Customer> CustomerWindow;
					HWND hh = Common::OpenWindowBackGround<CustomerWindow>::Do(h);
					CustomerWindow *x = (CustomerWindow *)GetWindowLongPtr(hh, GWLP_USERDATA);
					x->hResult = e->hEditCs;
					SetWindowPos(hh, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
					DWORD dwStyle = GetWindowLong(hh, GWL_STYLE);
					dwStyle &= ~(WS_MAXIMIZEBOX | WS_MINIMIZEBOX);
					SetWindowLong(hh, GWL_STYLE, dwStyle);
				}
				return TRUE;
			}
		}
		break;
	case WM_INITDIALOG:
		{			
			SetWindowLong(h, GWL_USERDATA, lParam);
			PacketWindow *e = (PacketWindow *)lParam;
			
			int height = -20;
			int dy = 30;
			static const int edit_width = 320;
			static const int label_width = 200;

			e->hNumber = CreateWindowEx(WS_EX_CLIENTEDGE, L"edit"
				, Singleton<NumberPacket>::Instance().value
				, WS_BORDER | WS_VISIBLE | WS_CHILD | ES_LEFT | WS_TABSTOP
				, 10, height += dy, edit_width, 25, h, 0, hInstance, NULL
				);
			
			CreateWindow(L"static", L"Номер пакета"
				, WS_VISIBLE | WS_CHILD
				, edit_width + 50, height + 3, label_width, 20, h, 0, hInstance, NULL
				);

			e->hEditOp = CreateWindowEx(WS_EX_CLIENTEDGE, L"edit"
				, Singleton<Operator>::Instance().value
				, WS_BORDER | WS_VISIBLE | WS_CHILD | ES_LEFT | WS_TABSTOP | ES_READONLY
				, 10, height += dy, edit_width, 25, h, 0, hInstance, NULL
				);
			CreateWindow(L"button", L"..."
				, WS_VISIBLE | WS_CHILD | WS_TABSTOP 
				,edit_width + 20, height, 25, 25, h, (HMENU)ID_Operator, hInstance, NULL
				);
			CreateWindow(L"static", L"Оператор"
				, WS_VISIBLE | WS_CHILD
				, edit_width + 50, height + 3, label_width, 20, h, 0, hInstance, NULL
				);

			e->hEditCs = CreateWindowEx(WS_EX_CLIENTEDGE, L"edit"
				, Singleton<Customer>::Instance().value
				, WS_BORDER | WS_VISIBLE | WS_CHILD | ES_LEFT | WS_TABSTOP | ES_READONLY
				, 10, height += dy, edit_width, 25, h, 0, hInstance, NULL
				);
			CreateWindow(L"button", L"..."
				, WS_VISIBLE | WS_CHILD | WS_TABSTOP 
				,edit_width + 20, height, 25, 25, h, (HMENU)ID_Customer, hInstance, NULL
				);
			CreateWindow(L"static", L"Заказчик"
				, WS_VISIBLE | WS_CHILD
				, edit_width + 50, height + 3, label_width, 20, h, 0, hInstance, NULL
				);

			static const int width = edit_width + label_width + 30;	

			height += 40;

			RECT r;
			GetWindowRect(GetDesktopWindow(), &r);

			int btnWidth = width / 2 - 20;

			CreateWindow(L"button", L"Продолжить"
				, WS_VISIBLE | WS_CHILD | WS_TABSTOP 
				, 10, height, btnWidth, 30, h, (HMENU)ID_OK, hInstance, NULL
				);

			CreateWindow(L"button", L"Отмена"
				, WS_VISIBLE | WS_CHILD | WS_TABSTOP 
				, 20 + btnWidth, height, btnWidth, 30, h, (HMENU)ID_CANCEL, hInstance, NULL
				);

			height += 65;
			int x = r.left +(r.right - r.left - width) / 2;
			int y = r.top +(r.bottom - r.top - height) / 2;
			MoveWindow(h, x, y, width, height, FALSE);
		}
		return TRUE;
	}
	return FALSE;
}

bool PacketWindow::Do(HWND h)
{
	return TemplDlg_Do(h, L"Пакет", (DLGPROC)Proc, (LPARAM)this);
}

bool PacketWindow::OkBtn()
{
   wchar_t buf[128];
   GetWindowText(hNumber, buf, 128);
   if(0 == buf[0])
   {
	  MessageBox(GetParent(hNumber), L"Номер пакета не заполнен", L"Ошибка!!!", MB_ICONHAND);
	  return false;
   }
   Singleton<NumberPacket>::Instance().value = buf;

   GetWindowText(hEditOp, buf, 128);
   Singleton<Operator>::Instance().value = buf;

   GetWindowText(hEditCs, buf, 128);
   Singleton<Customer>::Instance().value = buf;

   PacketBase::Save(
		Singleton<Operator>::Instance().value
		, Singleton<Customer>::Instance().value
		, Singleton<NumberPacket>::Instance().value
		);
   return true;
}