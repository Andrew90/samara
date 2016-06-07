#pragma once

struct Device1730
{
	bool Init(unsigned){return true;}
	void Destroy(){}
	unsigned Read(){return -1;}
	unsigned ReadOutput(){return -1;}
	void WriteOutput(unsigned){}
};
extern Device1730 device1730;