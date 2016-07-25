#include "stdafx.h"
#include "App.h"
#include "AppBase.h"
#include "Device1730.h"
#include "RestartService.h"
#include "DebugMess.h"

bool App::Init()
{
   AppBase().Init();
   if(!device1730.Init(Singleton<Descriptor1730Table>::Instance().items.get<Descriptor1730>().value))
   {
	   char b[128];
	   CharToOemA("�� ���� ������������ ����� 1730\n", b);
	   dprint(b);
	   return false;
   }
   device1730.Write(Singleton<OutputBitTable>::Instance().items.get<oPowerBM>().value);
   Sleep(2000);
   if(!RestartService())
   {
	   char b[128];
	   CharToOemA("��� ����� � �������������\n", b);
	   dprint(b);
	   return false;
   }
   return true;
}

void App::Destroy()
{
	if(device1730.IsOpen())
	{
		Sleep(2000);
		device1730.Write(0);
	}
}

Device1730 device1730;
