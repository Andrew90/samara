#pragma once
#include "config.h"
#include "WindowsPosition.h"
#include "Dialogs.h"
#include "USPCWindow.h"
#include "CrossThresholdWindow.h"
namespace MainWindowMenu
{
	struct MainFile{};
	MENU_TEXT(L"����", TopMenu<MainFile>)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	struct LoadDateFile{static void Do(HWND h){zprint("");}};
	struct SaveDateFile{static void Do(HWND h){zprint("");}};
	struct Compute     : RecalculationDlg{};//{static void Do(HWND h){zprint("");}};
	struct MainExit{static void Do(HWND h){DestroyWindow(h);}};

	MENU_ITEM(L"��������� ������", LoadDateFile)
	MENU_ITEM(L"��������� ������", SaveDateFile)
	MENU_ITEM(L"����������", Compute)
	MENU_ITEM(L"�����", MainExit)

	template<>struct TopMenu<MainFile>
	{
		typedef TL::MkTlst<
			MenuItem<LoadDateFile>
			, MenuItem<SaveDateFile>
			, Separator<0>
			, MenuItem<Compute>
			, Separator<1>
			, MenuItem<MainExit>
		>::Result list;
	};
	//------------------------------------------------------------------------
	struct MainOptionTypeSize{};
	MENU_TEXT(L"����������", TopMenu<MainOptionTypeSize>)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
	struct ThicknessTreshold : Common::OpenWindow<CrossThresholdWindow>{};//{static void Do(HWND h){zprint("");}};
	struct DeadZones         : DeadZonesDlg{};//{static void Do(HWND h){zprint("");}};
	struct AllowableThickness{static void Do(HWND h){zprint("");}};
	struct RotationalSpeed   {static void Do(HWND h){zprint("");}};
							 						
	struct MainCreateTypesize: AddTypeSizeDlg{};//{static void Do(HWND h){zprint("");}};
	struct MainDeleteTypeSize: DelTypeSizeDlg{};//{static void Do(HWND h){zprint("");}};
	
    MENU_ITEM(L"������ ����������", ThicknessTreshold)
	MENU_ITEM(L"̸����� ����", DeadZones)
	MENU_ITEM(L"���������� �������", AllowableThickness)
	MENU_ITEM(L"�������� ��������", RotationalSpeed)
	MENU_ITEM(L"������� ����������", MainCreateTypesize)
	MENU_ITEM(L"������� ����������", MainDeleteTypeSize)

	template<>struct TopMenu<MainOptionTypeSize>
	{
		typedef TL::MkTlst<
		     MenuItem<ThicknessTreshold>
			, MenuItem<DeadZones>
			, MenuItem<AllowableThickness>
			, MenuItem<RotationalSpeed>
			, Separator<0>
			, MenuItem<MainCreateTypesize>
			, MenuItem<MainDeleteTypeSize>
		>::Result list;
	 };
	//--------------------------------------------------------------------------------
	struct Options{};
	MENU_TEXT(L"���������", TopMenu<Options>)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	struct WindowPosition    : WindowPositionDlg<MainWindow>{};//{static void Do(HWND h){zprint("");}};
	struct IOportsView       : IOportsDlg{};

	MENU_ITEM(L"��������� ���������� ����", WindowPosition)
    MENU_ITEM(L"�������� ���������� ������", IOportsView)

	template<>struct TopMenu<Options>
	{
		typedef TL::MkTlst<
			MenuItem<WindowPosition>
			, Separator<0>
			, MenuItem<IOportsView>
		>::Result list;		
	};
	//-------------------------------------------------------------------------------------------------------
	struct Setting{};
	MENU_TEXT(L"���������", TopMenu<Setting>)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	struct AnalogPlate            : Common::OpenWindow<USPCWindow>{};//{static void Do(HWND h){zprint("");}};
	struct DiscretePlate	      {static void Do(HWND h){zprint("");}};
	struct DiscretePlateInputs    : InputsDlg{};//{static void Do(HWND h){zprint("");}};
	struct DiscretePlateOutputs   : OutputsDlg{};//{static void Do(HWND h){zprint("");}};
	struct ColorItems             {static void Do(HWND h){zprint("");}};
	struct DiscretePlateDescriptor: Descriptor1730Dlg{};//{static void Do(HWND h){zprint("");}};
	struct Coefficient            {static void Do(HWND h){zprint("");}};
	struct MedianFiltre           {static void Do(HWND h){zprint("");}};
	struct Signal                 {static void Do(HWND h){zprint("");}};
	struct ACFBorder              {static void Do(HWND h){zprint("");}};
	struct TestTab                : TestTabsDlg{};//{static void Do(HWND h){zprint("");}};
	
	MENU_TEXT(L"���������� �����", SubMenu<DiscretePlate>)
	MENU_ITEM(L"������� �����", DiscretePlateInputs)
	MENU_ITEM(L"������� �����", DiscretePlateOutputs)
	MENU_ITEM(L"���������� �����", DiscretePlateDescriptor)
	MENU_ITEM(L"������������ ���������", Coefficient)
	MENU_ITEM(L"������ ���������� �������", MedianFiltre)
	MENU_ITEM(L"���������� �����", AnalogPlate)
	MENU_ITEM(L"��������� �������", Signal)
	MENU_ITEM(L"������� ���", ACFBorder)
	MENU_ITEM(L"�����", ColorItems)
	MENU_ITEM(L"������������ ���� � ����������", TestTab)
	
	template<>struct SubMenu<DiscretePlate>
	{
		typedef TL::TypeToTypeLst<
			typename TL::MkTlst<DiscretePlateDescriptor, DiscretePlateInputs, DiscretePlateOutputs>::Result 
			, MenuItem
		>::Result list;
	};

	template<>struct TopMenu<Setting>
	{
		typedef TL::MkTlst<
			MenuItem<AnalogPlate>
			, SubMenu<DiscretePlate>	
			, MenuItem<Coefficient>
			, MenuItem<MedianFiltre>
			, MenuItem<Signal>
			, MenuItem<ACFBorder>
			, MenuItem<ColorItems>
			, MenuItem<TestTab>
		>::Result list;		
	};
	// ----------------------------------------------------------------------------------------------------
	struct MainAbout : AboutWindowDlg{};//{static void Do(HWND h){zprint("");}};
	MENU_TEXT(L"� ���������", TopMenu<MainAbout>)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	MENU_ITEM(L"� ���������", MainAbout)

	template<>struct TopMenu<MainAbout>
	{
		typedef TL::MkTlst<
			MenuItem<MainAbout>
		>::Result list;
	};
	// --------------------------------------------------------------------------------------------------------- 
	/// \brief	�������� ���� ������ �������������
	//struct TestMenu{};
	//MENU_TEXT(L"TestMenu", TopMenu<TestMenu>)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//struct TestSubMenu0{};
	//struct TestSubMenu1{static void Do(HWND){zprint("");}};
	//struct TestSubMenu2{static void Do(HWND){zprint("");}};
	//struct TestSubMenu3{static void Do(HWND){zprint("");}};
	//struct TestSubMenu4{static void Do(HWND){zprint("");}};
	//
	//MENU_ITEM(L"TestSubMenu0", TestSubMenu0)
	//MENU_ITEM(L"TestSubMenu1", TestSubMenu1)
	//MENU_ITEM(L"TestSubMenu2", TestSubMenu2)
	//MENU_ITEM(L"TestSubMenu3", TestSubMenu3)
	//MENU_ITEM(L"TestSubMenu4", TestSubMenu4)
	//
	//template<>struct TopMenu<TestMenu>
	//{
	//	typedef TL::MkTlst<
	//		MenuItem<TestSubMenu0>
	//		, MenuItem<TestSubMenu1>
	//		, MenuItem<TestSubMenu2>
	//		, MenuItem<TestSubMenu3>
	//		, MenuItem<TestSubMenu4>
	//	>::Result list;
	//};
	//---------------------------------------------------------------------------------------------------------------
	typedef TL::MkTlst<
		TopMenu<MainFile>
		, TopMenu<MainOptionTypeSize>
		, TopMenu<Options>
		, TopMenu<Setting>
		, TopMenu<MainAbout>
	>::Result MainMenu;	
	}
