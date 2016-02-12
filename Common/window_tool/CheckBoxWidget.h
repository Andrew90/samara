#pragma once
#include "message.h"
#include "MessagesInterface.h"
#include "DebugMess.h"

/**
* \brief ������ ��� �������� 
* \param T - ��������� �������� ������ ������������� ������  TestCheckBox
*/
template<class T>class CheckBoxWidget : public T, public TEvent
{
	HWND hWnd;
public:
	CheckBoxWidget(){}
	template<class Z>CheckBoxWidget(Z &z) : T(z){}
	/**
	* \brief ��������� ����� � ����������� WM_CREATE
	* \param hOwner - ������ winapi
	* \param txt - ����� ����� ������
	* \param bool T::Init(HWND) - ������������ ��������� ��������(true - ������� ��������, false - ���������)
	*/
	void Init(HWND hOwner, const wchar_t *txt)
	{
		hWnd = CreateWindow(L"button", txt
			, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | WS_TABSTOP
			, 0, 0, 0, 0, hOwner, NULL, GetModuleHandle(NULL), NULL
			);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG)this);
		Button_SetCheck(hWnd, T::Init(hWnd) ? BST_CHECKED : BST_UNCHECKED);
	}
	/**
	* \brief ��������� � ����������� WM_SIZE
	*/
	void Size(int left, int top, int width, int height)
	{
		MoveWindow(hWnd, left, top, width, height, true);
	}
	bool GetCheck(){return  BST_CHECKED == Button_GetCheck(hWnd);}
private:
	/**
	* \brief ����������� ��� ������ ��� ��������� ������� 
	* \param static void T::Command(bool) - ���������� � ��������� ���������
	*/
	void Do(TCommand &m)
	{
		T::Command(m, BST_CHECKED == Button_GetCheck(m.hControl));
	}
};