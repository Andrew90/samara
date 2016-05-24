#include "stdafx.h"
#include "CustomerWindow.h"
#include "DebugMess.h"
#include "ViewersMenu.hpp"
#include "PacketBase.h"

#pragma warning(disable : 4355)
CustomerWindow::CustomerWindow()
	: okBtn(*this)
	, cancelBtn(*this)
{}  

void CustomerWindow::operator()(TSize &l)
{
	if(l.resizing == SIZE_MINIMIZED || 0 == l.Width || 0 == l.Height) return;
	int y = l.Height - 50;
	grid.Size(0, 0, l.Width, y);

	int width = (l.Width - 30) / 2;

	MoveWindow(hOk, 10, y + 10, width, 30, false);
	MoveWindow(hCancel, 20 + width, y + 10, width, 30, false);
}

void CustomerWindow::operator()(TCommand &l)
{
	EventDo(l);
}	

VIEWERS_MENU(CustomerWindow)
unsigned CustomerWindow::operator()(TCreate &l)
{
	result[0] = '\0';
	hResult = NULL;
	Menu<ViewersMenuCustomerWindow::MainMenu>().Init(l.hwnd);
	gridViewer.SetRClick(this, &CustomerWindow::RClick);
	gridViewer.SetLClick(this, &CustomerWindow::LClick);
	gridViewer.SetData(&data);
	grid.Create(l, &gridViewer);
	SetRow(grid.hWnd, data.Count());

	hOk = CreateWindow(L"button", L"Применить"
					, WS_VISIBLE | WS_CHILD | WS_TABSTOP
					,0, 0, 0, 0, l.hwnd, NULL, hInstance, NULL
					);
	SetWindowLongPtr(hOk, GWLP_USERDATA, (LONG)&okBtn);

	hCancel = CreateWindow(L"button", L"Отмена"
					, WS_VISIBLE | WS_CHILD | WS_TABSTOP
					,0, 0, 0, 0, l.hwnd, NULL, hInstance, NULL
					);
	SetWindowLongPtr(hCancel, GWLP_USERDATA, (LONG)&cancelBtn);

	return 0;
}

void CustomerWindow::operator()(TDestroy &l)
{
	delete this;
	SetWindowLongPtr(l.hwnd, GWLP_USERDATA, NULL);
}

unsigned CustomerWindow::operator()(TNotify &l)
{
	return GridNotify::OwnerNotify(l);
}

void CustomerWindow::LClick(int col, wchar_t *s)
{
	wcsncpy(result, s, dimention_of(result));
}

namespace
{
	struct AddItem{};
	struct DelItem{};

	template<>struct TopMenu<AddItem>			   
	{										   
		typedef NullType list;				   
	};										   
	MENU_TEXT(L"Добавить", TopMenu<AddItem>)	   

	template<>struct TopMenu<DelItem>			   
	{										   
		typedef NullType list;				   
	};										   
	MENU_TEXT(L"Удалить", TopMenu<DelItem>)
											   
	typedef TL::MkTlst<						   
		TopMenu<AddItem>						   
		, TopMenu<DelItem>
	>::Result items_list;	

	template<class O, class P>struct __item__
	{
		void operator()(O *o, P *)
		{
			 o->value.value =  __data_from_widget__<O, typename TL::Inner<O>::Result::type_value>()(*o);
		}
	};

	struct OkBtn
	{
		static const int width = 120;
		static const int height = 30;
		static const int ID = IDOK;
		wchar_t *Title(){return L"Применить";}
		template<class Owner>void BtnHandler(Owner &o, HWND h)
		{	
			if(!TL::find<typename Owner::list, __test__>()(&o.items, &h))return;
			TL::foreach<typename Owner::list, __item__>()(&o.items, &h);
			EndDialog(h, TRUE);
		}
	};

	PARAM_TITLE(Customer, L"Заказчик")
	CHECK_EMPTY_STRING(Customer)
	template<int N>struct DlgSubItems<Customer, Holder<N> >: EditItems<Customer, 200>{};

	template<>struct Event<TopMenu<AddItem> >	   
	{										   
		static void Do(HWND h)				   
		{									   			
			CustomersTable ot;
			if(TemplDialog<CustomersTable, TL::MkTlst<OkBtn, CancelBtn>::Result>(ot).Do(h, L"Добавить оператора"))
			{
				CustomerWindow *w = (CustomerWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
				w->data.Add(ot.items.get<Customer>().value);
				SetRow(w->grid.hWnd, 1);
				UpdateRow(w->grid.hWnd);
			}
		}									   
	};		

	template<>struct Event<TopMenu<DelItem> >	   
	{										   
		static void Do(HWND h)				   
		{									   
			CustomerWindow *o = (CustomerWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
			if(o->data.Count() > 1)
			{
				o->result[0] = '\0';
				o->data.Del(o->result);
				UpdateRow(o->grid.hWnd);
			}
			else
			{
				MessageBox(h, L" Должна быть хотя бы одна запись", L"Предупреждение!!!", MB_ICONEXCLAMATION);
			}
		}									   
	};		
	typedef TL::MkTlst<
		TopMenu<AddItem>
		, TopMenu<DelItem>
	>::Result popup_items_list;
}

void CustomerWindow::RClick(int col, wchar_t *s)
{
	PopupMenu<popup_items_list>::Do(hWnd, hWnd);
}

wchar_t *CustomerWindow::Title()
{
	return L"Выбор заказчика";
}

CustomerWindow::OkBtn::OkBtn(CustomerWindow &owner)
	: owner(owner)
{}

void CustomerWindow::OkBtn::Do(TCommand &l)
{
	if('\0' != owner.result[0])
	{
		SetWindowText(owner.hResult, owner.result); 
		DestroyWindow(l.hwnd);
		return;
	}
	MessageBox(l.hwnd, L"Необходимо выбрать заказчика", L"Предупреждение!!!", MB_ICONEXCLAMATION);
}

CustomerWindow::CancelBtn::CancelBtn(CustomerWindow &owner)
	: owner(owner)
{}

void CustomerWindow::CancelBtn::Do(TCommand &l)
{
	DestroyWindow(l.hwnd);
}
