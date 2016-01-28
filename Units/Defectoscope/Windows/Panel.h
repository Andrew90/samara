#pragma once
class Panel
{
public:
	HWND hWnd;
	void operator()(TMessage &){};
};