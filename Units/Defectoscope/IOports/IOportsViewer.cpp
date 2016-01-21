#include "stdafx.h"
#include "IOportsViewer.h"
#include "ColorLabel.h"
#include "AppBase.h"
#include "DebugMess.h"
#include "Chart.h"
#include "Device1730.h"
#include "App.h"
//ClientEvents clientEvents;
using namespace Gdiplus;
//-----------------------------------------------------------------------------
namespace
{
struct __draw_data__
{
	int x, dY;
	Graphics &g;
	int colorOn;
	int value;
	__draw_data__(int x, Graphics &g, int colorOn, int value)  : x(x), dY(15), g(g), colorOn(colorOn), value(value) {}
};

template<class T>struct MessageField
{
	wchar_t buf[128];
	wchar_t *Text()
	{		
		wsprintf(buf, L"%S", &__FUNCTION__[43]);
		int len = wcslen(buf);
		buf[len - 7] = 0;
		return buf;
	}
};

template<class O, class P>struct __draw__
{
	void operator()(O *o, P *p)
	{
		ColorLabel l;
		l.left = p->x;
		l.top = p->dY;
		wsprintf(l.buffer, L"<55>%s",  MessageField<O>().Text());
		l.Draw(p->g);
		int color = 0xffcccccc;
		p->g.FillRectangle(&SolidBrush(Color(color)), p->x - 20, p->dY - 3, 15, 15);
		p->dY += 20;
	}
};
//------------------------------------------------------------------------------------
struct __update_data__
{
	int x, dY;
	HDCGraphics &g;
	int colorOn;
	int value;
	__update_data__(int x, HDCGraphics &g, int colorOn, int value)  : x(x), dY(15), g(g), colorOn(colorOn), value(value) {}
};
template<class O, class P>struct __update__
{
	void operator()(O *o, P *p)
	{
		int color = 0xffcccccc;
		if(o->value & p->value) color = p->colorOn;
		p->g.graphics.FillRectangle(&SolidBrush(Color(color)), p->x - 20, p->dY - 3, 15, 15);
		p->dY += 20;
	}
};
VOID CALLBACK __Update__(PVOID oo, BOOLEAN)
{
	IOportsViewer *o = (IOportsViewer *)oo;
	HDCGraphics g(o->hWnd, o->backScreen);
//	if(mainWindow.options)clientEvents.map->input = device1730.Read();
	TL::foreach<InputBitTable::items_list, __update__>()(&Singleton<InputBitTable>::Instance().items, &__update_data__(40, g, 0xff0000ff, 7));//clientEvents.map->input));
	TL::foreach<OutputBitTable::items_list, __update__>()(&Singleton<OutputBitTable>::Instance().items, &__update_data__(230, g, 0xffff0000, 5));//clientEvents.map->output));
}
}
//--------------------------------------------------------------------------------------
IOportsViewer::IOportsViewer(HWND &h, Bitmap *&bs)
	: hWnd(h)
	, backScreen(bs)
	, hTimer(NULL)
{}
//----------------------------------------------------------------------------------------
void IOportsViewer::Size(Graphics &g, int width, int height)
{
	g.FillRectangle(&SolidBrush(Color(0xffaaaaaa)), 0, 0, width, height); 
	TL::foreach<InputBitTable::items_list, __draw__>()(&Singleton<InputBitTable>::Instance().items, &__draw_data__(40, g, 0xff0000ff, 5));//clientEvents.map->input));
	TL::foreach<OutputBitTable::items_list, __draw__>()(&Singleton<OutputBitTable>::Instance().items, &__draw_data__(230, g, 0xffff0000, 6));//clientEvents.map->output));
}
//---------------------------------------------------------------------------------------
void IOportsViewer::Start()
{
//	clientEvents.map->output &= ~Singleton<OutputsTable>::Instance().items.get<SensorsSupplyY4BitOut>().value;
//	clientEvents.map->output |= Singleton<OutputsTable>::Instance().items.get<SensorsRemovalY3BitOut>().value;
//	if(mainWindow.options)device1730.Write();//clientEvents.map->output);
	CreateTimerQueueTimer(&hTimer, NULL, __Update__, this, 2000, 500, WT_EXECUTEDEFAULT);
}
//------------------------------------------------------------------------------------------
void IOportsViewer::Stop()
{
//	clientEvents.map->output &= ~Singleton<OutputsTable>::Instance().items.get<SensorsSupplyY4BitOut>().value;
//	clientEvents.map->output |= Singleton<OutputsTable>::Instance().items.get<SensorsRemovalY3BitOut>().value;
	//if(mainWindow.options)device1730.Write();//clientEvents.map->output);
	if(hTimer)DeleteTimerQueueTimer( NULL, hTimer, NULL );
}
//--------------------------------------------------------------------------------------------
struct __mouse_down_data__
{
	int x, dY;
	HDCGraphics &g;
	int colorOn;
	unsigned &value;
	TLButtonDown &l;
	__mouse_down_data__(int x, HDCGraphics &g, int colorOn, unsigned &value, TLButtonDown &l)  : x(x), dY(15), g(g), colorOn(colorOn), value(value), l(l) {}
};
template<class O, class P>struct __mouse_down__
{
	void operator()(O *o, P *p)
	{
		int x0 = p->x - 20;
		int x1 = x0 + 15;
		int y0 = p->dY - 3;
		int y1 = y0 + 15;
		if(p->l.x > x0 && p->l.x < x1 && p->l.y > y0 && p->l.y < y1)
		{
			dprint("mouse %d %d %s", p->l.x, p->l.y, typeid(O).name());
			p->value ^= o->value;
			device1730.Write();//p->value);
		}
		int color = 0xffcccccc;
		if(o->value & p->value) color = p->colorOn;
		p->g.graphics.FillRectangle(&SolidBrush(Color(color)), x0, y0, 15, 15);
		p->dY += 20;
	}
};
void __mouse_down_test_message(HWND h, unsigned inp)
{
	//wchar_t *sq1 = inp & Singleton<InputsTable>::Instance().items.get<PipeAfterClamp1SQ1BitIn>().value   ? MessageField<PipeAfterClamp1SQ1BitIn>::Text() : L"";
	//wchar_t *sq3 = inp & Singleton<InputsTable>::Instance().items.get<PipeInletSQ3BitIn>().value         ? MessageField<PipeInletSQ3BitIn>::Text() : L"";
	//wchar_t *sq4 = inp & Singleton<InputsTable>::Instance().items.get<OutletPipeSQ4BitIn>().value		 ? MessageField<OutletPipeSQ4BitIn>::Text() : L"";
	//wchar_t *sq7 = inp & Singleton<InputsTable>::Instance().items.get<PipeClamp2BeforeSQ7BitIn>().value	 ? MessageField<PipeClamp2BeforeSQ7BitIn>::Text() : L"";
	//wchar_t buf[1024];
	//wsprintf(buf, L"�������� ������\n%s\n%s\n%s\n%s", sq1, sq3, sq4, sq7);
	//MessageBox(h, buf, L"��������������", MB_ICONWARNING);
}
template<class O, class P>struct __mouse_down_test__
{
	void operator()(O *o, P *p)
	{
		int x0 = p->x - 20;
		int x1 = x0 + 15;
		int y0 = p->dY - 3;
		int y1 = y0 + 15;
		if(p->l.x > x0 && p->l.x < x1 && p->l.y > y0 && p->l.y < y1)
		{
			dprint("mouse %d %d %s", p->l.x, p->l.y, typeid(O).name());
			unsigned inp = device1730.Read();
			//const unsigned msk = Singleton<InputsTable>::Instance().items.get<PipeAfterClamp1SQ1BitIn>().value
			//	| Singleton<InputsTable>::Instance().items.get<PipeInletSQ3BitIn>().value
			//	| Singleton<InputsTable>::Instance().items.get<OutletPipeSQ4BitIn>().value
			//	| Singleton<InputsTable>::Instance().items.get<PipeClamp2BeforeSQ7BitIn>().value
			//	;
			//inp &= msk;
			if(0 == inp)
			{
				p->value ^= o->value;
				device1730.Write();//p->value);
			}
			else
			{
				__mouse_down_test_message(p->l.hwnd, inp);
			}
		}
		int color = 0xffcccccc;
		if(o->value & p->value) color = p->colorOn;
		p->g.graphics.FillRectangle(&SolidBrush(Color(color)), x0, y0, 15, 15);
		p->dY += 20;
	}
};
//template<class P>struct __mouse_down__<SensorUnitSupplyY2BitOut, P>: __mouse_down_test__<SensorUnitSupplyY2BitOut, P>{};
//template<class P>struct __mouse_down__<SensorUnitRemovalY1BitOut, P>: __mouse_down_test__<SensorUnitRemovalY1BitOut, P>{};
void IOportsViewer::MouseLDown(TLButtonDown &l)
{
	//HDCGraphics g(managementIOports.hWnd, managementIOports.backScreen);
	//TL::foreach<input_bits_list, __update__>()(&Singleton<InputBitsTable>::Instance().items, &__update_data__(40, g, 0xff0000ff, clientEvents.map->input));
	//TL::foreach<output_bits_list, __mouse_down__>()(&Singleton<OutputBitsTable>::Instance().items, &__mouse_down_data__(230, g, 0xffff0000, clientEvents.map->output, l));
}

