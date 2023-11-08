#include "stdin_channel.h"
#include <iostream>


stdin_channel::stdin_channel()
{
}


stdin_channel::~stdin_channel()
{
}

void stdin_channel::WriteFd(std::string _output)
{
}

int stdin_channel::GetFd()
{
	return 0;
}

bool stdin_channel::init()
{
	return true;
}

void stdin_channel::fini()
{
}

ZinxHandler * stdin_channel::GetInputNextStage(ByteMsg * _byte)
{
	return m_proc;
}

std::string stdin_channel::ReadFd()
{
	std::string ret;
	std::cin >> ret;
	return ret;
}
