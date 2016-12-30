#include "stdafx.h"
#include "ItemsData/ConstData.h"
#include "App/AppBase.h"
#include "DiffApp/App.h"
#include "MessageText\LabelMessage.h"
#include "debug_tools/DebugMess.h"
namespace ConstData{
	
	unsigned buf[TL::Length<ColorTable::items_list>::value];	

	template<class O, class P>struct __init__
	{
		void operator()(O *o, P *p)
		{
			static const int i = TL::IndexOf<ColorTable::items_list, O>::value;
			p[i] = o->value;
		}
	};

	void Init()
	{
		TL::foreach<ColorTable::items_list, __init__>()(&Singleton<ColorTable>::Instance().items, buf);
	}

	unsigned ZoneColor(unsigned c)
	{		
		if(c < dimention_of(buf))
		{
				return buf[c];
		}
		return 0xBADC0105;
	}
}