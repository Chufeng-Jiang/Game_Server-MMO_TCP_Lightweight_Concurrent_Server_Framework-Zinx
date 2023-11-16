#pragma once
#include "Ichannel.h"
#include <string>

class FIFO_channel :
	public Ichannel
{
	std::string fifo_name;
	int m_fd = -1;
	bool bIsRead = true;

public:
	FIFO_channel *m_out = NULL;
	FIFO_channel(std::string _file, bool _isRead);
	virtual ~FIFO_channel();

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

