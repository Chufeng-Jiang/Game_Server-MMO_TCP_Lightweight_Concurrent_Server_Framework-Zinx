#include "ZinxHandler.h"



ZinxHandler::ZinxHandler()
{
}


ZinxHandler::~ZinxHandler()
{
}

void ZinxHandler::handle(ZinxMsg * _inputMsg)
{
	/*1.当前环节处理*/
	auto pNextMsg = internel_handle(_inputMsg);
	if (nullptr != pNextMsg)
	{
		/*2.获取下一个环节*/
		ZinxHandler *pNextHanler = GetNext(pNextMsg);
		if (nullptr != pNextHanler)
		{
			/*3.下一个环节处理*/
			pNextHanler->handle(pNextMsg);
		}
		delete pNextMsg;
	}

}
