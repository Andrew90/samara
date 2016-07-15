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

extern Automat automat;

bool USPC_Test();

