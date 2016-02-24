#include "stdafx.h"
#include "CrossThresholdWindow.h"
#include "AppBase.h"

unsigned CrossThresholdWindow::operator()(TCreate &l)
{
	(*(Parent *)this)(l);

	memmove(brak
		, Singleton<ThresholdsTable>::Instance().items.get<BorderDefect<Cross> >().value
		, sizeof(brak)
		);
	memmove(klass2
		, Singleton<ThresholdsTable>::Instance().items.get<Border2Class<Cross> >().value
		, sizeof(klass2)
		);
	return 0;
}

void CrossThresholdWindow::operator()(TClose &l)
{
	bool brakChanged = false;
	double *data = Singleton<ThresholdsTable>::Instance().items.get<BorderDefect<Cross> >().value;
	for(int i = 0; i < dimention_of(brak); ++i)
	{
		if(data[i] != brak[i])
		{
			brakChanged = true;
			break;
		}
	}
	bool klass2Changed = false;
	data = Singleton<ThresholdsTable>::Instance().items.get<Border2Class<Cross> >().value;
	for(int i = 0; i < dimention_of(brak); ++i)
	{
		if(data[i] != klass2[i])
		{
			klass2Changed = true;
			break;
		}
	}
	if(brakChanged || klass2Changed)
	{
		int ret = MessageBox(l.hwnd, L"Данные были изменены!\nСохранить?", L"Cообщение", MB_ICONQUESTION | MB_YESNOCANCEL);
		if(IDYES == ret)
		{
			zprint("  if(brakChanged || klass2Changed)\n");
			//if(brakChanged)
			//{
				memmove(
					Singleton<ThresholdsTable>::Instance().items.get<BorderDefect<Cross> >().value
					, brak
					, sizeof(brak)
					);
			//}
			//if(klass2Changed)
			//{
				memmove(
					Singleton<ThresholdsTable>::Instance().items.get<Border2Class<Cross> >().value
					, klass2
					, sizeof(klass2)
					);
			//}
		}
		if(IDCANCEL == ret) return;
	}
	DestroyWindow(l.hwnd);
}

wchar_t *CrossThresholdWindow::Title()
{
	return L"Поперечные пороги";
}
