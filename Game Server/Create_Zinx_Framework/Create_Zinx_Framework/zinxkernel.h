#pragma once

#include "stdin_channel.h"
#include "Ichannel.h"
#include <list>

class zinxkernel
{
	stdin_channel *m_channel = nullptr;
	std::list<Ichannel *> m_channel_list;
	int m_epollFd = -1;
	zinxkernel();
	virtual ~zinxkernel();
	static zinxkernel kernel;
public:
	void run();
	void AddChannel(Ichannel *_pChannel);
	void DelChannel(Ichannel *_pChannel);
	void ModChannel_AddOut(Ichannel *_pChannel);
	void ModChannel_DelOut(Ichannel *_pChannel);
	stdin_channel *GetChannel()
	{
		return m_channel;
	}
	static zinxkernel &GetInstance()
	{
		return kernel;
	}
};

