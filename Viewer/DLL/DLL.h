// DLL.h

#pragma once
#include "Lib.h"

using namespace System;

namespace DLL {

	public ref class IChart abstract 
	{
	public:
		virtual void Create()													 abstract;
		virtual void Destroy()													 abstract;
		virtual void Size(IntPtr h, unsigned resizing, int width, int height)	 abstract;
		virtual void SetData(int, double *)										 abstract;
		virtual void SetStatus(int, char *)										 abstract;
		virtual void SetCountZones(int)											 abstract;
	};

	public ref class CrossChart: public IChart
	{	
		void *pimpl;
	public:	
		virtual void Create() override;
		virtual void Destroy() override;
		virtual void Size(IntPtr h, unsigned resizing, int width, int height) override;
		virtual void SetData(int, double *)override;
		virtual void SetStatus(int, char *)override;
		virtual void SetCountZones(int)override;
	};

	public ref class LongChart: public IChart
	{	
		void *pimpl;
	public:	
		virtual void Create()override;
		virtual void Destroy()override;
		virtual void Size(IntPtr h, unsigned resizing, int width, int height)override;
		virtual void SetData(int, double *)override;
		virtual void SetStatus(int, char *)override;
		virtual void SetCountZones(int)override;
	};

	public ref class ThicknessChart: public IChart
	{	
		void *pimpl;
	public:	
		virtual void Create()override;
		virtual void Destroy()override;
		virtual void Size(IntPtr h, unsigned resizing, int width, int height)override;
		virtual void SetData(int, double *)override;
		virtual void SetStatus(int, char *)override;
		virtual void SetCountZones(int)override;
		void SetMinMaxYAxes(double, double);
	};

	public ref class Additional
	{
	public:
		static void ChartsInit();
		static void RepaintWindow(IntPtr h);
	};
}
