#pragma once
#include "WindowsPosition.h"
#include "Dialogs.h"
namespace IOportsWindowMenu
{
struct MainFile{};
MENU_TEXT(L"����", TopMenu<MainFile>)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
struct MainExit{static void Do(HWND h){DestroyWindow(h);}};

MENU_ITEM(L"�����", MainExit)

template<>struct TopMenu<MainFile>
{
	typedef TL::MkTlst<
		MenuItem<MainExit>
	>::Result list;
};

/*
struct MainExit{};

*/
struct MainOptionUnits{};
MENU_TEXT(L"���������", TopMenu<MainOptionUnits>)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
struct WindowPosition : WindowPositionDlg<IOportsWindow>{};

MENU_ITEM(L"��������� ���������� ����", WindowPosition)

template<>struct TopMenu<MainOptionUnits>
{
	typedef TL::MkTlst<
		MenuItem<WindowPosition>
	>::Result list;
};

typedef TL::MkTlst<
	TopMenu<MainFile>
	, TopMenu<MainOptionUnits>
>::Result MainMenu;

/*
MENU_TEXT(L"�����", MenuItem<MainExit>)

MENU_TEXT(L"���������", TopMenu<MainOptionUnits>)
MENU_TEXT(L"��������� ���������� ����", MenuItem<ManagmentOptionsCoordinatesDlg>)
	

template<>struct Event<MenuItem<MainExit> >{static void Do(HWND h){DestroyWindow(h);}};//����� �� ���������
template<>struct Event<MenuItem<ManagmentOptionsCoordinatesDlg> >{static void Do(HWND h){ManagmentOptionsCoordinatesDlg::Do(h);}};//����� �� ���������
*/
}

