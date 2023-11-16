#include "FIFO_channel.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

FIFO_channel::FIFO_channel(std::string _file, bool _isRead):fifo_name(_file),bIsRead(_isRead)
{
}

FIFO_channel::~FIFO_channel()
{
}

std::string FIFO_channel::ReadFd()
{
	char szbuff[1024] = { 0 };

	int ilen = read(m_fd, szbuff, sizeof(szbuff));
	
	return std::string(szbuff, ilen);
}

void FIFO_channel::WriteFd(std::string _output)
{
	char *pout = (char *)malloc(_output.size());
	_output.copy(pout, _output.size(), 0);
	write(m_fd, pout, _output.size());
	free(pout);
}

int FIFO_channel::GetFd()
{
	return m_fd;
}

void FIFO_channel::data_process(std::string _input)
{
	m_out->data_sendout(_input);
}

bool FIFO_channel::init()
{
	bool bRet = false;
	//打开文件
	int flag = O_RDONLY;
	if (false == bIsRead)
	{
		flag = O_WRONLY;
	}
	int fd = open(fifo_name.c_str(), flag|O_NONBLOCK);
	if (fd >= 0)
	{
		m_fd = fd;
		bRet = true;
	}
	return bRet;
}

void FIFO_channel::fini()
{
	if (m_fd >= 0)
	{
		close(m_fd);
	}
}
