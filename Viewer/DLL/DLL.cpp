// This is the main DLL file.

#include "stdafx.h"

#include "DLL.h"
namespace DLL {
	void CrossChart::Create()
	{
		pimpl = (void *)::CrossChart::Create();
	}

	void CrossChart::Destroy()
	{
		::CrossChart::Destroy((::CrossChart *)pimpl);
	}

	void CrossChart::Size(IntPtr h, unsigned resizing, int width, int height)
	{
		((::CrossChart *)pimpl)->Size((HWND)(h.ToInt32()), 0, width, height);
	}

	void CrossChart::SetData(int channel, double *data)
	{
		((::CrossChart *)pimpl)->SetData(channel, data);
	}

	void CrossChart::SetStatus(int channel, char *status)
	{
		((::CrossChart *)pimpl)->SetStatus(channel, status);
	}

	void CrossChart::SetCountZones(int countZones)
	{
		((::CrossChart *)pimpl)->SetCountZones(countZones);
	}
//--------------------------------------------------------------------------------------------
	void LongChart::Create()
	{
		pimpl = (void *)::LongChart::Create();
	}

	void LongChart::Destroy()
	{
		::LongChart::Destroy((::LongChart *)pimpl);
	}

	void LongChart::Size(IntPtr h, unsigned resizing, int width, int height)
	{
		((::LongChart *)pimpl)->Size((HWND)(h.ToInt32()), 0, width, height);
	}

	void LongChart::SetData(int channel, double *data)
	{
		((::LongChart *)pimpl)->SetData(channel, data);
	}

	void LongChart::SetStatus(int channel, char *status)
	{
		((::LongChart *)pimpl)->SetStatus(channel, status);
	}

	void LongChart::SetCountZones(int countZones)
	{
		((::LongChart *)pimpl)->SetCountZones(countZones);
	}
//--------------------------------------------------------------------------------------------
	void ThicknessChart::Create()
	{
		pimpl = (void *)::ThicknessChart::Create();
	}

	void ThicknessChart::Destroy()
	{
		::ThicknessChart::Destroy((::ThicknessChart *)pimpl);
	}

	void ThicknessChart::Size(IntPtr h, unsigned resizing, int width, int height)
	{
		((::ThicknessChart *)pimpl)->Size((HWND)(h.ToInt32()), 0, width, height);
	}

	void ThicknessChart::SetData(int channel, double *data)
	{
		((::ThicknessChart *)pimpl)->SetData(channel, data);
	}

	void ThicknessChart::SetStatus(int channel, char *status)
	{
		((::ThicknessChart *)pimpl)->SetStatus(channel, status);
	}

	void ThicknessChart::SetCountZones(int countZones)
	{
		((::ThicknessChart *)pimpl)->SetCountZones(countZones);
	}
	void ThicknessChart::SetMinMaxYAxes(double min, double max)
	{
		((::ThicknessChart *)pimpl)->SetMinMaxYAxes(min, max);
	}
//-----------------------------------------------------------------------------------------
	void Additional::ChartsInit()
	{
		::ChartsInit();
	}

	void Additional::RepaintWindow(IntPtr h)
	{
		::RepaintWindow((HWND)(h.ToInt32()));
	}
}
