#include "stdafx.h"
#include "tUSPC.h"
#include <Windows.h>
#include "..\..\Common\USPC\uspc7100_exports.h"
#pragma comment(lib, "..\\..\\Common\\USPC\\USPC7100.lib")


USPC7100_ASCANDATAHEADER data[1024];

void TestUSPC()
{
	int id = 0;
	unsigned err = 0;
	err = USPC7100_Open(2);
	printf("USPC7100_Open %x\n", err);
	err = USPC7100_Load(-1, -1, "2.us");
	printf("USPC7100_Load %x\n", err);

	ULONG numberOfScansAcquired, numberOfScansRead, bufferSize, scanSize;
	ULONG status;
	err =  USPC7100_Acq_Get_Status(id, &status, &numberOfScansAcquired, &numberOfScansRead, &bufferSize, &scanSize);
	printf("USPC7100_Acq_Get_Status %x\n", err);

    ULONG conditions[8] = {};
	int fluidity = 64;
	err = USPC7100_Acq_Config(id, 0x1000, 1, conditions, 0, 1
		, 1024 * 64, &fluidity, NULL 
		);
	printf("USPC7100_Acq_Config %x\n", err);

    err = USPC7100_Acq_Start(id, -1);
	printf("USPC7100_Acq_Start %x\n", err);

	Sleep(1000);

	ULONG numberRead;
	ULONG scansBacklog;

	UCHAR *pData = (UCHAR *)data;

	//err = USPC7100_Acq_Clear(id);
	//printf("USPC7100_Acq_Clear %x\n", err);

	for(int i = 0; i < 100; ++i)
	{
		err = USPC7100_Acq_Read(
					id
					, -1
					, 0
					, &numberRead
					, &scansBacklog
					, pData
					);
		printf("USPC7100_Acq_Read %x\n", err);
		printf("numberRead %d\n", numberRead);
        printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
		for(int j = 0; j < (int)numberRead; ++j)
		{
			USPC7100_ASCANDATAHEADER *d = &data[j];
			if(d->Channel != 1) continue;
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
			Sleep(200);
			//err = USPC7100_Acq_Clear(id);
	        //printf("USPC7100_Acq_Clear %x\n", err);
			break;
		}		
	}

	err = USPC7100_Acq_Stop(id);
	printf("USPC7100_Acq_Stop %x\n", err);

	err = USPC7100_Close();
	printf("USPC7100_Close %x\n", err);
}