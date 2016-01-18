#include "stdafx.h"
#include "AppBase.h"
#include <typeinfo>
#include "DebugMess.h"
//-----------------------------------------------------------------------
void AppBase::Init()
{
	
}
//------------------------------------------------------------------------
const wchar_t *ParametersBase::name()
{
	GetModuleFileName(0, path, dimention_of(path));
	PathRemoveFileSpec(path);
	wcscat(path, L"\\ParametersBase");
	return path;
}
//-------------------------------------------------------------------------	






