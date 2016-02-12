#pragma once
#include "WindowsPosition.h"
#include "Dialogs.h"

#define VIEWERS_MENU(name)namespace ViewersMenu##name				   \
{															   \
	struct MainFile{};										   \
	MENU_TEXT(L"Файл", TopMenu<MainFile>)					   \
		   \
	struct MainExit{static void Do(HWND h){DestroyWindow(h);}};\
															   \
	MENU_ITEM(L"Выход", MainExit)							   \
															   \
	template<>struct TopMenu<MainFile>						   \
	{														   \
		typedef TL::MkTlst<									   \
			 MenuItem<MainExit>								   \
		>::Result list;										   \
	};														   \
	   \
	struct Options{};										   \
	MENU_TEXT(L"Настройки", TopMenu<Options>)				   \
	  \
	struct WindowPosition    : WindowPositionDlg<name>{};	   \
															   \
	MENU_ITEM(L"Сохранить координаты окна", WindowPosition)	   \
															   \
	template<>struct TopMenu<Options>						   \
	{														   \
		typedef TL::MkTlst<									   \
			MenuItem<WindowPosition>						   \
		>::Result list;										   \
	};														   \
	   \
	typedef TL::MkTlst<										   \
		TopMenu<MainFile>									   \
		, TopMenu<Options>									   \
	>::Result MainMenu;										   \
	}

#define CONTEXT_MENU(name)namespace ContextMenu##name\
{											   \
	template<>struct TopMenu<name>			   \
	{										   \
		typedef NullType list;				   \
	};										   \
	MENU_TEXT(L"Просмотр", TopMenu<name>)	   \
											   \
	typedef TL::MkTlst<						   \
		TopMenu<name>						   \
	>::Result items_list;					   \
											   \
											   \
	template<>struct Event<TopMenu<name> >	   \
	{										   \
		static void Do(HWND h)				   \
		{									   \
			zprint("\n");					   \
			name::Do(h);					   \
		}									   \
	};										   \
};
