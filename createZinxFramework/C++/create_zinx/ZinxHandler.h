#pragma once

class ZinxMsg {
public:
	ZinxMsg() {}
	virtual ~ZinxMsg() {}
};

class ZinxHandler
{
public:
	ZinxHandler();
	virtual ~ZinxHandler();
	void handle(ZinxMsg *_inputMsg);
	virtual ZinxMsg *internel_handle(ZinxMsg *_inputMsg) = 0;
	//_next_input应该是internel_handle的返回值
	virtual ZinxHandler *GetNext(ZinxMsg *_next_input) = 0;
};

