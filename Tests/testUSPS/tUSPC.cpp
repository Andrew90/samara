#include "stdafx.h"
#include "tUSPC.h"
#include <Windows.h>
#include "..\..\Common\USPC\uspc7100_exports.h"
#pragma comment(lib, "..\\..\\Common\\USPC\\USPC7100.lib")


USPC7100_ASCANDATAHEADER data0[1024];
USPC7100_ASCANDATAHEADER data1[1024];
USPC7100_ASCANDATAHEADER data2[1024];

void Print(int id, USPC7100_ASCANDATAHEADER *d)
{
			printf("ScanCounter %d Channel %d count %d time %d\n", 
				d->hdr.ScanCounter
				, d->hdr.Channel
				, d->DataSize
				, d->TimeEqu
				);
			for(int i = 0; i < d->DataSize; ++i)
			{
				printf("%d ", d->Point[i]);
			}
			printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
}

void TestUSPC()
{
	unsigned err = 0;
	err = USPC7100_Open(2);
	printf("USPC7100_Open %x\n", err);
	err = USPC7100_Load(-1, -1, "2.us");
	printf("USPC7100_Load %x\n", err);

	for(int id0 = 0; id0 < 3; ++id0)
	{
	ULONG numberOfScansAcquired, numberOfScansRead, bufferSize, scanSize;
	ULONG status;
	err =  USPC7100_Acq_Get_Status(id0, &status, &numberOfScansAcquired, &numberOfScansRead, &bufferSize, &scanSize);
	printf("USPC7100_Acq_Get_Status %x\n", err);
	}

	for(int id0 = 0; id0 < 3; ++id0)
	{
    ULONG conditions[8] = {};
	int fluidity = 64;
	err = USPC7100_Acq_Config(id0, 0x1000, 1, conditions, 0, 1
		, 1024 * 64, &fluidity, NULL 
		);
	printf("USPC7100_Acq_Config %x\n", err);
	}
for(int id0 = 0; id0 < 3; ++id0)
{
    err = USPC7100_Acq_Start(id0, -1);
	printf("USPC7100_Acq_Start %x\n", err);
}
	Sleep(1000);

	ULONG numberRead;
	ULONG scansBacklog;

	UCHAR *pData = (UCHAR *)data0;

	for(int i = 0; i < 100; ++i)
	{
		for(int id = 0; id < 3; ++id)
		{
			Sleep(50);
		err = USPC7100_Acq_Read(
					id
					, -1
					, 0
					, &numberRead
					, &scansBacklog
					, (UCHAR *)data0
					);
		printf("USPC7100_Acq_Read %d %x\n", id, err);
		printf("numberRead %d\n", numberRead);
		}
#if 0
        //printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
		// Print(0, data0);
		//printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
		// Print(1, data1);
		//printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
		// Print(2, data2);
		//printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
	}
#endif
	for(int id0 = 0; id0 < 3; ++id0)
	{
	err = USPC7100_Acq_Stop(id0);
	printf("USPC7100_Acq_Stop %x\n", err);
	}
	err = USPC7100_Close();
	printf("USPC7100_Close %x\n", err);
}