#pragma once

namespace ViewerMenu
{
	struct DetailedView{};
	template<>struct TopMenu<DetailedView>
	{
		typedef NullType list;
	};
	MENU_TEXT(L"Просмотр", TopMenu<DetailedView>)

	typedef TL::MkTlst<
		TopMenu<DetailedView>
	>::Result items_list;


	template<>struct Event<TopMenu<DetailedView> >
	{
		static void Do(HWND)
		{
			zprint("\n");
		}
	};
};

#define DETALIED_VIEW(name)namespace name##ViewerMenu\
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