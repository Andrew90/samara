#include "stdafx.h"
#include "PacketBase.h"

const wchar_t *PacketBase::name()
{
	GetModuleFileName(0, path, dimention_of(path));
	int len = wcslen(path) - 4;
	wcscpy(&path[len], L"Packet");
	return path;
}

void PacketBase::Restore(wchar_t *op_, wchar_t *cs_, wchar_t *num_)
{
	PacketBase parameters;
	CBase base(
		parameters.name()
		, CreateDataBase<PacketBase::type_list, SetDefault<PacketBase::type_list> >()
		, parameters.tables
		);
	if(base.IsOpen())
	{
		CurrentPacketTable current;
		Select<CurrentPacketTable>(base).ID(1).Execute(current);

		OperatorsTable op;
		Select<OperatorsTable>(base).ID(current.items.get<ID<Operator>>().value).Execute(op);
		wcscpy(op_, op.items.get<Operator>().value);

		CustomersTable cs;
		Select<CustomersTable>(base).ID(current.items.get<ID<Customer>>().value).Execute(cs);
		wcscpy(cs_, cs.items.get<Customer>().value);

		NumberPacketsTable num;
		Select<NumberPacketsTable>(base).ID(1).Execute(num);
		wcscpy(num_, num.items.get<NumberPacket>().value);
	}
}

void PacketBase::Save(wchar_t *op_, wchar_t *cs_, wchar_t *num_)
{
	PacketBase parameters;
	CBase base(
		parameters.name()
		, CreateDataBase<PacketBase::type_list, SetDefault<PacketBase::type_list> >()
		, parameters.tables
		);
	if(base.IsOpen())
	{
		OperatorsTable op;
		op.items.get<Operator>().value = op_;
		unsigned idOp = Select<OperatorsTable>(base).eq<Operator>(op.items.get<Operator>().value).Execute();
		if(0 == idOp)
		{
			Insert_Into<OperatorsTable>(op, base).Execute();
			idOp = Select<OperatorsTable>(base).eq<Operator>(op.items.get<Operator>().value).Execute();
		}

		CustomersTable cs;
		cs.items.get<Customer>().value = cs_;
		unsigned idCs = Select<CustomersTable>(base).eq<Customer>(cs.items.get<Customer>().value).Execute();
		if(0 == idCs)
		{
			Insert_Into<CustomersTable>(cs, base).Execute();
			idCs = Select<CustomersTable>(base).eq<Customer>(cs.items.get<Customer>().value).Execute();
		}

		NumberPacket::type_value numPcketValue;
		numPcketValue = num_;
		Update<NumberPacketsTable>(base).set<NumberPacket>(numPcketValue).Where().ID(1).Execute();

		Update<CurrentPacketTable>(base).set<ID<Operator>>(idOp).set<ID<Customer>>(idCs).Where().ID(1).Execute();
	}
}

