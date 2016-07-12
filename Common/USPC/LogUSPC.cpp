#include "stdafx.h"
#include "LogUSPC.h"
#include <stdio.h>
#include "Registry.h"
#include "DebugMess.h"
//wchar_t *path = L"D:\\Shared\\USPC7100log.txt";
void LogUSPC::Clear()
{
	RegistryPathLogFile t;
	wchar_t *path = t();
	int fSuccess = DeleteFile(path);
	if (!fSuccess) 
    {
        // Handle the error.
       dprint("DeleteFile failed (%d)\n", GetLastError());
    }
}
void LogUSPC::Open()
{
	offs = 0;
	RegistryPathLogFile t;
	wchar_t *path = t();
	FILE *f;
	if(!_wfopen_s(&f, path, L"r"))
	{
		int i = 0;
		while(true)
		{
			int k = i % 128;
			if(!fgets(buffer[k], 128, f))break;
			if(' ' == buffer[k][0])continue;
			++i;
		}
		for(;i < 128; ++i) buffer[i][0] = '\0';
		if(i > 128) offs = i % 128;
		fclose(f);
	}
}
 char *LogUSPC::operator[](int i)
 {
	 int k = (offs + i) % 128;
	 return buffer[k];
 }
