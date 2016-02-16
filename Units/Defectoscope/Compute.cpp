#include "stdafx.h"
#include "Compute.h"
#include "USPCData.h"
#include "Dialogs.h"

Compute::Compute()
{
}

namespace
{
	void ComputeData(USPCViewerData &d)
	{
		USPC7100_ASCANDATAHEADER *b = d.ascanBuffer;
		ZeroMemory(d.buffer, sizeof(d.buffer));

		for(int i = 1; i < d.currentOffsetZones; ++i)
		{
			for(int j = d.offsets[i - 1], last = d.offsets[i]; j < last; ++j)
			{
				WORD channel = b[j].Channel;
				if(channel < App::count_sensors)
				{
					if(d.buffer[channel][i] < b[j].hdr.G1Amp)
					{
						d.buffer[channel][i] = b[j].hdr.G1Amp;
						d.status[channel][i] = rand() % 6 + 1;
					}
				}
			}
		}
	}

	void ComputeData(USPCViewerThicknessData &d)
	{
		USPC7100_ASCANDATAHEADER *b = d.ascanBuffer;
		ZeroMemory(d.zonesMax, sizeof(d.bufferMax));

		for(int i = 0; i < App::zonesCount; ++i)
			d.zonesMin[i] = 1000;

		for(int i = 0; i < d.currentOffsetZones; ++i)
		{
			double x = 0.1 * (rand() % 120) + 3; 
			if(d.zonesMax[i] < x)
			{
				d.zonesMax[i] = x;
			}
			if(d.zonesMin[i] > x)
			{
				d.zonesMin[i] = x * 0.7;
			}
			d.commonStatus[i] = rand() % 6 + 1;
		}
	}
}

void Compute::Recalculation()
{
	ComputeData(Singleton<ItemData<Long> >::Instance());
	ComputeData(Singleton<ItemData<Cross> >::Instance());
	ComputeData(Singleton<ItemData<Thickness> >::Instance());
	app.MainWindowUpdate();
}

Compute compute;

void RecalculationDlg::Do(HWND)
{
	compute.Recalculation();
}