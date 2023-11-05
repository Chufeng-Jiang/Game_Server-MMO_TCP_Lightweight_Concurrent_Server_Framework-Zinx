#include "GameChannel.h"
#include "GameRole.h"

GameChannel::GameChannel(int _fd) :ZinxTcpData(_fd)
{
}

GameChannel::~GameChannel()
{
}

AZinxHandler* GameChannel::GetInputNextStage(BytesMsg& _oInput)
{
	return m_proto;
}

ZinxTcpData* GameConnFact::CreateTcpDataChannel(int _fd)
{
/*创建tcp通道对象*/
	auto pChannel = new GameChannel(_fd);
/*创建协议对象*/
	auto pProtocol = new GameProtocol();
	/*创建玩家对象*/
	auto pRole = new GameRole();

	/*绑定协议对象和通道对象*/
	pChannel->m_proto = pProtocol;
	pProtocol->m_channel = pChannel;

	/*绑定协议对象和玩家对象*/
	pProtocol->m_Role = pRole;
	pRole->m_pProto = pProtocol;

/*将协议对象添加到kernel, 注意参数需要为指针*/
	ZinxKernel::Zinx_Add_Proto(*pProtocol);

	/*将玩家对象添加到kernel*/
	ZinxKernel::Zinx_Add_Role(*pRole);

	return pChannel;
}
