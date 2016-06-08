#include "stdafx.h"
#include "Process.h"
//#include <tlhelp32.h>
//---------------------------------------------------------------------------
bool IsProcessRun( wchar_t *processName )
{
  // HANDLE hSnapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
  //
  // PROCESSENTRY32 pe;
  // pe.dwSize = sizeof( PROCESSENTRY32 );
  // Process32First( hSnapshot, &pe );
  //
  // while(true) {
  //     if( 0 == wcscmp( pe.szExeFile, processName )) return true;
  //     if( !Process32Next( hSnapshot, &pe ) ) return false;
  // }
	return false;
}

bool ProcessRun(wchar_t *processName, wchar_t *path)
{
	//STARTUPINFO si = { sizeof(si) };
	//PROCESS_INFORMATION pi;	
	//return 0 != CreateProcess(
	//	processName, path, NULL, NULL, FALSE,
	//	NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi
	//	);
	return false;
}