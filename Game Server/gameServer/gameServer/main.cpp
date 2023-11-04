#include "GameChannel.h"

int main()
{
	ZinxKernel::ZinxKernelInit();
	/*添加监听通道*/
	ZinxKernel::Zinx_Add_Channel(*(new ZinxTCPListen(8899, new GameConnFact())));
	ZinxKernel::Zinx_Run();
	ZinxKernel::ZinxKernelFini();
}