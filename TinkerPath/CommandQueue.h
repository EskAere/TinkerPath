#pragma once
#include <queue>
#include "Command.h"

/* A FIFO container of commands*/
class CommandQueue
{
public:
	void						push(const Command& command);
	Command						pop();
	bool						isEmpty() const;


private:
	std::queue<Command>			mQueue;
};

