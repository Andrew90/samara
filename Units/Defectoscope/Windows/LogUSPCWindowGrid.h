#pragma once
#include "GridNotify.hpp"

class LogUSPCWindow;
class LogUSPCWindowGrid
{
public :
	struct Handlers
	{
		LogUSPCWindow *owner;
		void operator()(TCellData &);
		void operator()(TCellColor &);
	};
	HWND hWnd;
	LogUSPCWindowGrid();
	GridEventHandler<Handlers> handlers;
	void Init(HWND );
};