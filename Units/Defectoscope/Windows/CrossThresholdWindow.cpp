#include "stdafx.h"
#include "CrossThresholdWindow.h"
#include "AppBase.h"
#include "ParamDlg.h"
#include "ParamDlg.hpp"

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
	ThresholdsTable &table = Singleton<ThresholdsTable>::Instance();
	bool changedDefect = false;
	double *data = table.items.get<BorderDefect<Cross> >().value;
	for(int i = 0; i < dimention_of(brak); ++i)
	{
		if(data[i] != brak[i])
		{
			changedDefect = true;
			break;
		}
	}
	bool changedKlass2 = false;
	data = table.items.get<Border2Class<Cross> >().value;
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
		int ret = MessageBox(l.hwnd, L"������ ���� ��������!\n���������?", L"C��������", MB_ICONQUESTION | MB_YESNOCANCEL);
		if(IDYES == ret)
		{
			if(TestPassword<ThresholdsTable>()(l.hwnd))
			{
				if(changedDefect)
				{
					memmove(
						table.items.get<BorderDefect<Cross> >().value
						, brak
						, sizeof(brak)
						);
				}
				if(changedKlass2)
				{
					memmove(
						table.items.get<Border2Class<Cross> >().value
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
						if(changedDefect)  update.set<BorderDefect<Cross> >(brak);
						if(changedKlass2)  update.set<Border2Class<Cross> >(klass2);
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

wchar_t *CrossThresholdWindow::Title()
{
	return L"���������� ������";
}