#include "stdou_channel.h"
#include <iostream>

using namespace std;


stdou_channel::stdou_channel()
{
}


stdou_channel::~stdou_channel()
{
}

std::string stdou_channel::ReadFd()
{
	return std::string();
}

void stdou_channel::WriteFd(std::string _output)
{
	cout << _output << endl;
}

int stdou_channel::GetFd()
{
	return 1;
}

void stdou_channel::data_process(std::string _input)
{
}

bool stdou_channel::init()
{
	return true;
}

void stdou_channel::fini()
{
}
