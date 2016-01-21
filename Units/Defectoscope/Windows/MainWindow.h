#pragma once
#include "SelectTypeSizeList.h"
#include <windows.h>
#include "message.h"
#include "TopLabelViewer.h"
#include "CheckBoxWidget.h"
#include "MainWindowToolBar.h"
#include <CommCtrl.h>
#include "ThicknessViewer.h"
#include "CrossViewer.h"
#include "LongViewer.h"
#include "ResultViewer.h"

template<int N>struct TestCheckBoxX
{
protected:
	static void Command(TCommand &m, bool b)
	{
		MainWindow *o = (MainWindow *) GetWindowLongPtr(m.hwnd, GWLP_USERDATA);
		wchar_t text[128];
		wsprintf(text, L"Чекбокс %d   %s "
			, N
			, b ? L"Включён"
			    : L"Отключён"
			);
		SendMessage(o->hStatusWindow, SB_SETTEXT, N, (LONG)text);
	}
	bool Init(HWND h)
	{
		bool b = true;
		HWND hParent = GetParent(GetParent(h));
		MainWindow *o = (MainWindow *) GetWindowLongPtr(hParent, GWLP_USERDATA);
		wchar_t text[128];
		wsprintf(text, L"Чекбокс %d   %s "
			, N
			, b ? L"Включён"
			    : L"Отключён"
			);
		SendMessage(o->hStatusWindow, SB_SETTEXT, N, (LONG)text);
		return b;
	}

};

class MainWindow
{
public:
	HWND hWnd;
	HWND hStatusWindow;
	MainWindowToolBar toolBar;
	CheckBoxWidget<TestCheckBoxX<0> > testCheckBox0;
	CheckBoxWidget<TestCheckBoxX<1> > testCheckBox1;
	CheckBoxWidget<TestCheckBoxX<2> > testCheckBox2;
	TopLabelViewer topLabelViewer;
	SelectTypeSizeList select;
	typedef TL::MkTlst<
		CrossViewer
		, LongViewer
		, ThicknessViewer
		, ResultViewer
	>::Result viewers_list;
	TL::Factory<viewers_list> viewers;
	MainWindow();
	void operator()(TSize &);
	void operator()(TCommand &);
	void operator()(TGetMinMaxInfo &);
	unsigned operator()(TCreate &);
	void operator()(TDestroy &);
	void operator()(TMessage &);
	void operator()(TKeyDown &);
	void operator()(TRButtonDown &);
	void operator()(TMouseWell &);
};
