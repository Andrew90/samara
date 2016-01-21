#pragma once
#include "WindowsPosition.h"
#include "Dialogs.h"
namespace IOportsWindowMenu
{
struct MainFile{};
MENU_TEXT(L"Файл", TopMenu<MainFile>)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
struct MainExit{static void Do(HWND h){DestroyWindow(h);}};

MENU_ITEM(L"Выход", MainExit)

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
MENU_TEXT(L"Настройки", TopMenu<MainOptionUnits>)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
struct WindowPosition : WindowPositionDlg<IOportsWindow>{};

MENU_ITEM(L"Сохранить координаты окна", WindowPosition)

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
MENU_TEXT(L"Выход", MenuItem<MainExit>)

MENU_TEXT(L"Настройки", TopMenu<MainOptionUnits>)
MENU_TEXT(L"Сохранить координаты окна", MenuItem<ManagmentOptionsCoordinatesDlg>)
	

template<>struct Event<MenuItem<MainExit> >{static void Do(HWND h){DestroyWindow(h);}};//выход из программы
template<>struct Event<MenuItem<ManagmentOptionsCoordinatesDlg> >{static void Do(HWND h){ManagmentOptionsCoordinatesDlg::Do(h);}};//выход из программы
*/
}

