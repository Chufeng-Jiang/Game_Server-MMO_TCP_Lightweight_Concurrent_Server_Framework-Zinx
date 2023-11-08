#pragma once
#include <string>
#include "Ichannel.h"
#include "stdou_channel.h"
#include "ProcessFunc.h"

class stdin_channel:public Ichannel
{
	
public:
	stdin_channel();
	virtual ~stdin_channel();
	ProcessFunc *m_proc = NULL;

	// 通过 Ichannel 继承
	virtual std::string ReadFd() override;
	virtual void WriteFd(std::string _output) override;
	virtual int GetFd() override;

	// 通过 Ichannel 继承
	virtual bool init() override;
	virtual void fini() override;

	// 通过 Ichannel 继承
	virtual ZinxHandler * GetInputNextStage(ByteMsg * _byte) override;
};

