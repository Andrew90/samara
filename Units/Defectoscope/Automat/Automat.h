#pragma once

class Automat
{
public:
	struct Impl;
private:
	Impl &impl;
public:
	Automat();
	void Init();
};