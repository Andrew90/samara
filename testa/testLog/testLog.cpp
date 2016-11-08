// testLog.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string.h>

#pragma warning(disable : 4996)
int _tmain(int argc, _TCHAR* argv[])
{
	char b[128];
	FILE *f = fopen(".\\USPC7100log.txt", "wt");
	char line[] = "              \n";
	if(f)
	{
	for(int i = 0; i < 5000; ++i)
	{
		sprintf(b, "%05d stroka ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 123\n", i);
		int len =strlen(b);
		fwrite(b, 1, len, f);
		fwrite(line, 1, sizeof(line)- 1, f);
	}
	fclose(f);
	}
	return 0;
}

