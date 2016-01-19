#include "stdafx.h"
#include "AppBase.h"
#include <typeinfo>
#include "App.h"
#include "DebugMess.h"
//-----------------------------------------------------------------------
namespace
{
template<class O, class P>struct row_table
{
	void operator()(O *, P *p)
	{
		O &o = Singleton<O>::Instance();
		Select<O>(*p).ID(1).Execute(o);
	}
};
}
void AppBase::Init()
{
	ParametersBase parameters;
	CBase base(
		parameters.name()
		, CreateDataBase<ParametersBase::type_list, SetDefault<ParametersBase::type_list> >()
		, parameters.tables
		);
	if(base.IsOpen())
	{
		TL::foreach<ParametersBase::one_row_table_list, row_table>()((TL::Factory<ParametersBase::one_row_table_list> *)0, &base);
	}
}
//------------------------------------------------------------------------
const wchar_t *ParametersBase::name()
{
	GetModuleFileName(0, path, dimention_of(path));
	int len = wcslen(path);
	path[len - 4] = 0;
	return path;
}
//-------------------------------------------------------------------------	






