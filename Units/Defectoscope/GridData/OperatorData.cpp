#include "stdafx.h"
#include <algorithm>
#include "OperatorData.h"
#include "PacketBase.h"
#include "Grid.h"

OperatorData::OperatorData()
{
	Init();
}

template<class Table, class Data>struct OperatorData::__load__
{
	bool operator()(unsigned, Table &table, Data &data)
	{
		data.data.push_back(table.items.get<Operator>().value.buffer);
		return false;
	}
};

void OperatorData::Init()
{
	CBase base(PacketBase().name());
	if(base.IsOpen())
	{
		Select<OperatorsTable>(base).ExecuteLoop<__load__>(*this);
	}
}

void OperatorData::Add(wchar_t *s)
{
	CBase base(PacketBase().name());
	if(base.IsOpen())
	{
		Operator::type_value val;
		val = s;
		if(0 == Select<OperatorsTable>(base).eq<Operator>(val).Execute())
		{
			OperatorsTable t;
			t.items.get<Operator>().value = s;
			Insert_Into<OperatorsTable>(t, base).Execute();
			data.push_back(s);
		}
	}
}

namespace
{
	struct Compare
	{
		wchar_t *w;
		Compare(wchar_t *w) : w(w){}
		bool operator()(std::wstring &s) { return 0 == wcscmp(s.c_str(), w); }
	};
}

void OperatorData::Del(wchar_t *s)
{
	CBase base(PacketBase().name());
	if(base.IsOpen())
	{
		Operator::type_value val;
		val = s;
		unsigned id = Select<OperatorsTable>(base).eq<Operator>(val).Execute();
		if(0 != id)
		{			
			Delete<OperatorsTable>(base).ID(id).Execute();			
			data.erase(remove_if(data.begin(), data.end(), Compare(s)), data.end());
		}
	}
}

const wchar_t *OperatorData::Cell(int i, int)
{
	return data[i].c_str();
}

unsigned OperatorData::Count()
{
	return data.size();
}

namespace
{
    HEADER_TABLE(Operator, 600, Оператор);
	typedef TL::MkTlst<
		Operator
	>::Result header_list;
}

void OperatorData::Header(HWND h)
{
	 SetGridHeader<header_list> _(h);	
}

