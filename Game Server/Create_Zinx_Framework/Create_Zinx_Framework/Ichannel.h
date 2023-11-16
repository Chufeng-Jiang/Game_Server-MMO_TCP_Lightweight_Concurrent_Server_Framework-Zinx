#pragma once
#include <string>

class Ichannel
{
	std::string m_buffer;
public:
	Ichannel();
	virtual ~Ichannel();
	virtual bool init() = 0;
	virtual void fini() = 0;
	virtual std::string ReadFd() = 0;
	virtual void WriteFd(std::string _output) = 0;
	virtual int GetFd() = 0;
	virtual void data_process(std::string _input) = 0;

	void flushout();
	void data_sendout(std::string _output);
};

