#include "GameChannel.h"
#include "GameMsg.h"
#include "msg.pb.h"
#include "AOIWorld.h"
#include "ZinxTimer.h"
#include "RandomName.h"

extern RandomName random_name;

int main()
{
	random_name.LoadFile();
	ZinxKernel::ZinxKernelInit();
	/*添加监听通道*/
	ZinxKernel::Zinx_Add_Channel(*(new ZinxTCPListen(8889, new GameConnFact())));
	ZinxKernel::Zinx_Add_Channel(*(new ZinxTimerChannel()));
	ZinxKernel::Zinx_Run();
	ZinxKernel::ZinxKernelFini();
}