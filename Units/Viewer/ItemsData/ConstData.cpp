#include "stdafx.h"
#include "ConstData.h"
#include "AppBase.h"
#include "App.h"
#include "LabelMessage.h"
#include "DebugMess.h"
namespace ConstData{
	
	unsigned buf[TL::Length<ColorTable::items_list>::value];	

	template<class O, class P>struct __init__
	{
		void operator()(O *o, P *p)
		{
			p[TL::IndexOf<ColorTable::items_list, O>::value] = o->value;
		}
	};

	void Init()
	{
		TL::foreach<ColorTable::items_list, __init__>()(&Singleton<ColorTable>::Instance().items, buf);
	}

	unsigned ZoneColor(unsigned c)
	{
		if(c < dimention_of(buf)) return buf[c];
		if(c == TL::IndexOf<label_message_list, Xlr<BrackStrobe> >::value) 
			return Singleton<ColorTable>::Instance().items.get<Xlr<BrackStrobe>>().value;
		return 0xBADC0105;
	}
}