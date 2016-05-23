#include "stdafx.h"
#include <algorithm>
#include "CustomerData.h"
#include "PacketBase.h"
#include "Grid.h"

CustomerData::CustomerData()
{
	Init();
}

template<class Table, class Data>struct CustomerData::__load__
{
	bool operator()(unsigned, Table &table, Data &data)
	{
		data.data.push_back(table.items.get<Customer>().value.buffer);
		return false;
	}
};

void CustomerData::Init()
{
	CBase base(PacketBase().name());
	if(base.IsOpen())
	{
		Select<CustomersTable>(base).ExecuteLoop<__load__>(*this);
	}
}

void CustomerData::Add(wchar_t *s)
{
	CBase base(PacketBase().name());
	if(base.IsOpen())
	{
		Customer::type_value val;
		val = s;
		if(0 == Select<CustomersTable>(base).eq<Customer>(val).Execute())
		{
			CustomersTable t;
			t.items.get<Customer>().value = s;
			Insert_Into<CustomersTable>(t, base).Execute();
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

void CustomerData::Del(wchar_t *s)
{
	CBase base(PacketBase().name());
	if(base.IsOpen())
	{
		Customer::type_value val;
		val = s;
		unsigned id = Select<CustomersTable>(base).eq<Customer>(val).Execute();
		if(0 != id)
		{			
			Delete<CustomersTable>(base).ID(id).Execute();			
			data.erase(remove_if(data.begin(), data.end(), Compare(s)), data.end());
		}
	}
}

const wchar_t *CustomerData::Cell(int i, int)
{
	return data[i].c_str();
}

unsigned CustomerData::Count()
{
	return data.size();
}

namespace
{
    HEADER_TABLE(Customer, 600, Заказчик);
	typedef TL::MkTlst<
		Customer
	>::Result header_list;
}

void CustomerData::Header(HWND h)
{
	 SetGridHeader<header_list> _(h);	
}

