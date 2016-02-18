#include "stdafx.h"
#include "DataViewer.h"
#include "Frame.h"


namespace
{
	static const int count_buffer = 3000;
	struct PTR
	{
		USPC7100_ASCANDATAHEADER *data[count_buffer];
	};
	struct Data
	{
		double data[count_buffer];
	};
}

DefectData::DefectData()
	: data(NULL)
	, scan(NULL)
{}

void DefectData::Set(int start, int stop, int channel, int offs, int maxOffs, USPC7100_ASCANDATAHEADER *s)
{
	if(NULL == data)data = (double *)Frame::Get<Data>();
	if(NULL == scan)scan = (USPC7100_ASCANDATAHEADER **)Frame::Get<PTR>();
	stop += offs;
	int cnt = 0;
	if(stop > maxOffs) stop = maxOffs;
	for(int i = start + offs; i < stop; ++i)
	{
		if(channel == s[i].Channel)
		{
			data[cnt] = s[i].hdr.G2Amp;
			scan[cnt] = &s[i];
			++cnt;
		}
	}
	count = cnt;
}

void DefectData::Drop()
{
	Frame::Drop(data);
	Frame::Drop(scan);
	data = NULL;
	scan = NULL;
}