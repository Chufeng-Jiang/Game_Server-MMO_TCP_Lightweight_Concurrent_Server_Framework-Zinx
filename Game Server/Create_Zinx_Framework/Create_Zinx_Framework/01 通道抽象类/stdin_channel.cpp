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

void stdin_channel::data_process(std::string _input)
{
	//将数据回显到标准输出--->调用标准输出通道对象的sendout函数
	m_out->data_sendout(_input);
}

bool stdin_channel::init()
{
	return true;
}

void stdin_channel::fini()
{
}

std::string stdin_channel::ReadFd()
{
	std::string ret;
	std::cin >> ret;
	return ret;
}
