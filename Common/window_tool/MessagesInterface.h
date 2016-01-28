#pragma once

struct TCommand;
class TCommandEvent
{
public:
	virtual void Do(TCommand &) = 0;
};