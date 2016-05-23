#pragma once
#include "StoredBase.h"

struct CurrentPacketTable
{
	typedef TL::MkTlst<
		ID<Operator>
		, ID<Customer>
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"CurrentPacketTable";}
};

struct PacketBase
 {
	 typedef TL::MkTlst<		
		  OperatorsTable
		 , CustomersTable
		 , NumberPacketsTable
		 , CurrentPacketTable
	 >::Result type_list;
	 typedef TL::Factory<type_list> TTables;
	 TTables tables;
	  wchar_t path[512];
	 const wchar_t *name();
	 static void Restore(wchar_t *op, wchar_t *cs, wchar_t *num);
	 static void Save(wchar_t *op, wchar_t *cs, wchar_t *num);
 };