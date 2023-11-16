#pragma once
#include "Ichannel.h"
class stdou_channel :
	public Ichannel
{
public:
	stdou_channel();
	virtual ~stdou_channel();

	// 通过 Ichannel 继承
	virtual std::string ReadFd() override;
	virtual void WriteFd(std::string _output) override;
	virtual int GetFd() override;
	virtual void data_process(std::string _input) override;

	// 通过 Ichannel 继承
	virtual bool init() override;
	virtual void fini() override;
};

