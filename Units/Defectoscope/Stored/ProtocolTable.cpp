#include "stdafx.h"
#include "ProtocolTable.h"
#include "PacketBase.h"

namespace
{
	template<class O, class P>struct __set_data_table__
	{
		void operator()(O *o)
		{
			o->value = Singleton<O>::Instance().value;
		}
	};
}

unsigned GetProtocolID(CBase &b)
{
	ProtocolsTable t;
	TL::foreach<ProtocolsTable::items_list, __set_data_table__>()(&t.items);
	unsigned id = Select<ProtocolsTable>(b).eq_all<ProtocolsTable::items_list>(&t.items).Execute();
	if(0 == id)
	{
		Insert_Into<ProtocolsTable>(t, b).Execute();
		id = Select<ProtocolsTable>(b).eq_all<ProtocolsTable::items_list>(&t.items).Execute();
	}
	return id;
}