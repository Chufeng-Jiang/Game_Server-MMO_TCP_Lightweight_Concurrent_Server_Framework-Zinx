#include "zinxkernel.h"
#include <sys/epoll.h>
#include <unistd.h>
#include <iostream>

using namespace std;

zinxkernel zinxkernel::kernel;

zinxkernel::zinxkernel()
{
	int fd = epoll_create(1);
	if (fd >= 0)
	{
		m_epollFd = fd;
	}
}

zinxkernel::~zinxkernel()
{
	if (-1 != m_epollFd)
	{
		close(m_epollFd);
	}
}

void zinxkernel::run()
{
	while (1)
	{
		//1等输入（epoll）
		struct epoll_event astEvents[200];
		int ready_count = epoll_wait(m_epollFd, astEvents, 200, -1);
		if (0 == ready_count)
		{
			if (EINTR == errno)
			{
				continue;
			}
		}
		//2调用通道的读取函数或写出函数
		for (int i = 0; i < ready_count; i++)
		{
			if (0 != (astEvents[i].events & EPOLLIN))
			{
				//需要处理外部输入的数据
				/*1.读出数据*/
				auto pchannel = static_cast<Ichannel *> (astEvents[i].data.ptr);
				auto content = pchannel->ReadFd();
				/*2.处理数据*/
				pchannel->data_process(content);
			}
			if (0 != (astEvents[i].events & EPOLLOUT))
			{
				//需要向外输出的数据
				auto pchannel = static_cast<Ichannel *> (astEvents[i].data.ptr);
				pchannel->flushout();
				//删掉输出方向的epoll监听
				ModChannel_DelOut(pchannel);
			}
		}
	}	
}

void zinxkernel::AddChannel(Ichannel * _pChannel)
{
	if (true == _pChannel->init())
	{
		//将参数和文件描述符0关联起来（epoll_ctl）
		struct epoll_event stEvent;
		stEvent.events = EPOLLIN;
		stEvent.data.ptr = _pChannel;

		epoll_ctl(m_epollFd, EPOLL_CTL_ADD, _pChannel->GetFd(), &stEvent);
	}
}

void zinxkernel::DelChannel(Ichannel * _pChannel)
{
	epoll_ctl(m_epollFd, EPOLL_CTL_DEL, _pChannel->GetFd(), NULL);
	_pChannel->fini();
}

void zinxkernel::ModChannel_AddOut(Ichannel * _pChannel)
{
	struct epoll_event stEvent;
	stEvent.events = EPOLLIN | EPOLLOUT;
	stEvent.data.ptr = _pChannel;
	epoll_ctl(m_epollFd, EPOLL_CTL_MOD, _pChannel->GetFd(), &stEvent);
}

void zinxkernel::ModChannel_DelOut(Ichannel * _pChannel)
{
	struct epoll_event stEvent;
	stEvent.events = EPOLLIN;
	stEvent.data.ptr = _pChannel;
	epoll_ctl(m_epollFd, EPOLL_CTL_MOD, _pChannel->GetFd(), &stEvent);
}
