#include "stdafx.h"
#include "LongThresholdWindow.h"
#include "AppBase.h"
#include "ParamDlg.h"
#include "ParamDlg.hpp"

unsigned LongThresholdWindow::operator()(TCreate &l)
{
	(*(Parent *)this)(l);

	memmove(brak
		, Singleton<ThresholdsTable>::Instance().items.get<BorderDefect<Long> >().value
		, sizeof(brak)
		);
	memmove(klass2
		, Singleton<ThresholdsTable>::Instance().items.get<Border2Class<Long> >().value
		, sizeof(klass2)
		);
	return 0;
}

void LongThresholdWindow::operator()(TClose &l)
{
	ThresholdsTable &table = Singleton<ThresholdsTable>::Instance();
	bool changedDefect = false;
	double *data = table.items.get<BorderDefect<Long> >().value;
	for(int i = 0; i < dimention_of(brak); ++i)
	{
		if(data[i] != brak[i])
		{
			changedDefect = true;
			break;
		}
	}
	bool changedKlass2 = false;
	data = table.items.get<Border2Class<Long> >().value;
	for(int i = 0; i < dimention_of(brak); ++i)
	{
		if(data[i] != klass2[i])
		{
			changedKlass2 = true;
			break;
		}
	}
	if(changedDefect || changedKlass2)
	{
		int ret = MessageBox(l.hwnd, L"Данные были изменены!\nСохранить?", L"Cообщение", MB_ICONQUESTION | MB_YESNOCANCEL);
		if(IDYES == ret)
		{
			if(TestPassword<ThresholdsTable>()(l.hwnd))
			{
				if(changedDefect)
				{
					memmove(
						table.items.get<BorderDefect<Long> >().value
						, brak
						, sizeof(brak)
						);
				}
				if(changedKlass2)
				{
					memmove(
						table.items.get<Border2Class<Long> >().value
						, klass2
						, sizeof(klass2)
						);
				}
				CBase base(ParametersBase().name());
				if(base.IsOpen())
				{
					int id = CurrentId<ID<ThresholdsTable> >();

					if(1 == CountId<ID<ThresholdsTable> >(base, id))
					{
						Update<ThresholdsTable> update(base);
						if(changedDefect)  update.set<BorderDefect<Long> >(brak);
						if(changedKlass2)  update.set<Border2Class<Long> >(klass2);
						update.Where().ID(id).Execute();
					}
					else
					{
						Insert_Into<ThresholdsTable>(table, base).Execute();
						int id = Select<ThresholdsTable>(base).eq_all<ThresholdsTable::items_list>(&table.items).Execute();
						UpdateId<ID<ThresholdsTable> >(base, id);
					}
				}
			}
		}
		if(IDCANCEL == ret) return;
	}
	DestroyWindow(l.hwnd);
}

wchar_t *LongThresholdWindow::Title()
{
	return L"Продольные пороги";
}
