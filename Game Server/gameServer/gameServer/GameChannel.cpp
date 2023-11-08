#include "GameChannel.h"
#include "GameRole.h"


GameChannel::GameChannel(int _fd):ZinxTcpData(_fd)
{
}


GameChannel::~GameChannel()
{
	if (NULL != m_proto)
	{
		ZinxKernel::Zinx_Del_Proto(*m_proto);
		delete m_proto;
	}
}

AZinxHandler * GameChannel::GetInputNextStage(BytesMsg & _oInput)
{
	return m_proto;
}

ZinxTcpData * GameConnFact::CreateTcpDataChannel(int _fd)
{
	/*����tcpͨ������*/
	auto pChannel = new GameChannel(_fd);
	/*����Э�����*/
	auto pProtocol = new GameProtocol();
	/*������Ҷ���*/
	auto pRole = new GameRole();

	/*��Э������ͨ������*/
	pChannel->m_proto = pProtocol;
	pProtocol->m_channel = pChannel;

	/*��Э��������Ҷ���*/
	pProtocol->m_Role = pRole;
	pRole->m_pProto = pProtocol;

	/*��Э�������ӵ�kernel*/
	ZinxKernel::Zinx_Add_Proto(*pProtocol);

	/*����Ҷ�����ӵ�kernel*/
	ZinxKernel::Zinx_Add_Role(*pRole);

	return pChannel;
}
