#include "GameChannel.h"
#include "GameMsg.h"
#include "msg.pb.h"

int main()
{
	pb::SyncPid* pmsg = new pb::SyncPid();
	pmsg->set_pid(1);
	pmsg->set_username("test");

	GameMsg gm(GameMsg::MSG_TYPE_LOGIN_ID_NAME, pmsg);
	auto output = gm.serialize();

	for (auto byte : output)
	{
		printf("%02X ", byte);
	}
	puts("");

	
	char buff[] = { 0x08, 0x01, 0x12, 0x04 ,0x74, 0x65, 0x73, 0x74 };
	std::string input(buff, sizeof(buff));

	auto ingm = GameMsg(GameMsg::MSG_TYPE_LOGIN_ID_NAME, input);
	std::cout << dynamic_cast<pb::SyncPid*> (ingm.pMsg)->pid() << std::endl;
	std::cout << dynamic_cast<pb::SyncPid*> (ingm.pMsg)->username() << std::endl;
	

	ZinxKernel::ZinxKernelInit();
	/*添加监听通道*/
	ZinxKernel::Zinx_Add_Channel(*(new ZinxTCPListen(8899, new GameConnFact())));
	ZinxKernel::Zinx_Run();
	ZinxKernel::ZinxKernelFini();
}