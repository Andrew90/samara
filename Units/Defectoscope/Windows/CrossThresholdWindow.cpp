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

namespace
{
	template<class O, class P>struct __store__
	{
		void operator()(O *o, P *p)
		{
			dprint("%s", __FUNCTION__);
			p->update.set<O>(o->value);
		}
	};
}

void CrossThresholdWindow::operator()(TClose &l)
{
	ThresholdsTable &table = Singleton<ThresholdsTable>::Instance();
	bool changed = false;
	double *data = table.items.get<BorderDefect<Cross> >().value;
	for(int i = 0; i < dimention_of(brak); ++i)
	{
		if(data[i] != brak[i])
		{
			changed = true;
			break;
		}
	}
	
	if(!changed)
	{
		data = table.items.get<Border2Class<Cross> >().value;
		for(int i = 0; i < dimention_of(brak); ++i)
		{
			if(data[i] != klass2[i])
			{
				changed = true;
				break;
			}
		}
	}
	if(changed)
	{
		int ret = MessageBox(l.hwnd, L"Данные были изменены!\nСохранить?", L"Cообщение", MB_ICONQUESTION | MB_YESNOCANCEL);
		if(IDYES == ret)
		{
			if(TestPassword<ThresholdsTable>()(l.hwnd))
			{
				zprint("  if(brakChanged || klass2Changed)\n");
				memmove(
					table.items.get<BorderDefect<Cross> >().value
					, brak
					, sizeof(brak)
					);
				memmove(
					table.items.get<Border2Class<Cross> >().value
					, klass2
					, sizeof(klass2)
					);
				CBase base(ParametersBase().name());
				if(base.IsOpen())
				{
					__update_data__<ThresholdsTable> _data(base);
					typedef TL::MkTlst<BorderDefect<Cross>, Border2Class<Cross> >::Result lst;
					TL::foreach<lst, __store__>()(&table.items, &_data);
					_data.update.Where().ID(1).Execute();
				}
			}
		}
		if(IDCANCEL == ret) return;
	}
		DestroyWindow(l.hwnd);
}

wchar_t *CrossThresholdWindow::Title()
{
	return L"Поперечные пороги";
}
