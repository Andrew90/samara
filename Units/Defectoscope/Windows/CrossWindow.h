#pragma once
#include "CrossViewer.h"
#include "LineViewer.h"
#include "typelist.hpp"

class CrossWindow
{
public:
	HWND hWnd;
	template<int N>struct Line: LineViewer
	{
		static const int ID = N;
	};
	typedef TL::AppendList<
		    TL::MkTlst<CrossViewer>::Result
		    , TL::CreateNumList<Line, 0, 7>::Result
	    >::Result viewers_list;
	TL::Factory<viewers_list> viewers;
	CrossWindow();
	void operator()(TSize &);
	void operator()(TCommand &);
	void operator()(TGetMinMaxInfo &);
	unsigned operator()(TCreate &);
	void operator()(TMouseWell &);
	static void Do(HWND);
	bool Destroy();
	static CrossWindow &Instance();
};