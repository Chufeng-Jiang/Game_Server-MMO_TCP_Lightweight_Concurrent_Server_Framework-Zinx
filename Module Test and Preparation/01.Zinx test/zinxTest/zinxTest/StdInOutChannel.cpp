#include "StdInOutChannel.h"
#include <iostream>
#include "CmdCheck.h"

using namespace std;

StdInChannel::StdInChannel()
{
}


StdInChannel::~StdInChannel()
{
}

bool StdInChannel::Init()
{
	return true;
}

bool StdInChannel::ReadFd(std::string& _input)
{
	cin >> _input;
	return true;
}

bool StdInChannel::WriteFd(std::string& _output)
{
	return false;
}

void StdInChannel::Fini()
{
}

int StdInChannel::GetFd()
{
	return 0;
}

std::string StdInChannel::GetChannelInfo()
{
	return "stdin";
}

AZinxHandler* StdInChannel::GetInputNextStage(BytesMsg& _oInput)
{
	/*返回协议对象*/
	return CmdCheck::GetInstance();
}


/// /////////////////////////////////////////////////////////////////////////

bool StdOutChannel::Init()
{
	return true;
}

bool StdOutChannel::ReadFd(std::string& _input)
{
	return false;
}

bool StdOutChannel::WriteFd(std::string& _output)
{
	cout << _output << endl;
	return true;
}

void StdOutChannel::Fini()
{
}

int StdOutChannel::GetFd()
{
	return 1;
}

std::string StdOutChannel::GetChannelInfo()
{
	return "stdout";
}

AZinxHandler* StdOutChannel::GetInputNextStage(BytesMsg& _oInput)
{
	return nullptr;
}

AZinxHandler* myTcpData::GetInputNextStage(BytesMsg& _oInput)
{
	/*返回协议对象*/
	return CmdCheck::GetInstance();
}

ZinxTcpData* myFact::CreateTcpDataChannel(int _fd)
{
	return new myTcpData(_fd);
}
