#pragma once
#include <string>
#include "ZinxHandler.h"

class sysIODicMsg :public ZinxMsg {
public:
	enum IO_DIC {
		IO_IN,IO_OUT
	} m_DIC;
	sysIODicMsg(IO_DIC _dic):m_DIC(_dic) {
	}
};

class ByteMsg :public sysIODicMsg {
public:
	std::string content;
	ByteMsg(std::string _content, sysIODicMsg &_iodic):content(_content),sysIODicMsg(_iodic.m_DIC) {}
};

class Ichannel:public ZinxHandler
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

	void flushout();
	void data_sendout(std::string _output);

	virtual ZinxHandler *GetInputNextStage(ByteMsg *_byte) = 0;

	// Í¨¹ý ZinxHandler ¼Ì³Ð
	virtual ZinxMsg * internel_handle(ZinxMsg * _inputMsg) override;
	virtual ZinxHandler * GetNext(ZinxMsg * _next_input) override;
};

