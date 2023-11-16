#pragma once
#include <string>
#include "Ichannel.h"
#include "stdou_channel.h"
class stdin_channel:public Ichannel
{
	
public:
	stdin_channel();
	virtual ~stdin_channel();
	stdou_channel *m_out = NULL;

	// 通过 Ichannel 继承
	virtual std::string ReadFd() override;
	virtual void WriteFd(std::string _output) override;
	virtual int GetFd() override;
	virtual void data_process(std::string _input) override;

	// 通过 Ichannel 继承
	virtual bool init() override;
	virtual void fini() override;
};

