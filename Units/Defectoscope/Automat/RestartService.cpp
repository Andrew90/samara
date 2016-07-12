#include "stdafx.h"
#include "RestartService.h"
#include "GetHostByName.h"
#include "ServiceProtocol.h"

namespace
{
	const wchar_t *hostName = L"ALCOA-BURAN5000";
}

bool RestartService()
{
	GetHostByName getHostByName;
	char status[128];
	status[0] = '\0';

	StopProtocol::Client::Do(getHostByName((wchar_t *)hostName), 8989);
	Sleep(500);
	for(int i = 0; i < 10; ++i)
	{
		if(StatusProtocol::Client::Do(getHostByName((wchar_t *)hostName), 8989, status))
		{
			return false;
		}
		if(0 == strcmp(status, "Stopped")) break;
		Sleep(500);
	}

	if(0 != strcmp(status, "Stopped")) return false;

	RunProtocol::Client::Do(getHostByName((wchar_t *)hostName), 8989);
	Sleep(500);
	for(int i = 0; i < 10; ++i)
	{
		if(StatusProtocol::Client::Do(getHostByName((wchar_t *)hostName), 8989, status))
		{
			return false;
		}
		if(0 == strcmp(status, "Running")) break;
		Sleep(500);
	}
	return 0 == strcmp(status, "Running");
}