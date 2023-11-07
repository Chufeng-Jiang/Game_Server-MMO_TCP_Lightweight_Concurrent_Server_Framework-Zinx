#include "GameProtocol.h"
#include <iostream>
#include "GameMsg.h"
#include "GameChannel.h"
#include "msg.pb.h"
#include "GameRole.h"
using namespace std;

GameProtocol::GameProtocol()
{
}

GameProtocol::~GameProtocol()
{
	if (NULL != m_Role)
	{
		ZinxKernel::Zinx_Del_Role(*m_Role);
		delete m_Role;
	}
}

/*输入参数是通道传来的原始报文
返回值是转换后的消息对象MultiMsg，
报文里面有多条请求，但是返回只能返回一条，
转换方式,TCP粘包处理*/
UserData* GameProtocol::raw2request(std::string _szInput)
{
	MultiMsg* pRet = new MultiMsg(); //此时没有用户请求
	szLast.append(_szInput);

	while (1)
	{
		if (szLast.size() < 8)
		{
			break;
		}

		/*在前四个字节中读取消息内容长度*/
		int iLength = 0;
		iLength |= szLast[0] << 0;
		iLength |= szLast[1] << 8;
		iLength |= szLast[2] << 16;
		iLength |= szLast[3] << 24;
		/*中四个字节读类型id*/
		int id = 0;
		id |= szLast[4] << 0;
		id |= szLast[5] << 8;
		id |= szLast[6] << 16;
		id |= szLast[7] << 24;

		/*通过读到的长度判断后续报文是否合法*/
		if (szLast.size() - 8 < iLength)
		{
			/*本条报文还没够，啥都不干*/
			break;
		}

		/*构造一条用户请求*/
		GameMsg* pMsg = new GameMsg((GameMsg::MSG_TYPE)id, szLast.substr(8, iLength)); // iLength是正文的长度
		pRet->m_Msgs.push_back(pMsg);

		/*弹出已经处理成功的报文*/
		szLast.erase(0, 8 + iLength);
	}

	//Debug打印每条请求
	/*
	for (auto single : pRet->m_Msgs)
	{
		cout << single->pMsg->Utf8DebugString() << endl;
	}


	pb::Talk* pmsg = new pb::Talk();
	pmsg->set_content("hello");
	GameMsg* pGameMsg = new GameMsg(GameMsg::MSG_TYPE_CHAT_CONTENT, pmsg);

	ZinxKernel::Zinx_SendOut(*(pGameMsg), *this);
	*/

	return pRet;
}

/*参数来自业务层，待发送的消息
返回值转换后的字节流*/
std::string* GameProtocol::response2raw(UserData& _oUserData)
{
	int iLength = 0;
	int id = 0;
	std::string MsgContent;

	GET_REF2DATA(GameMsg, oOutput, _oUserData);
	id = oOutput.enMsgType;
	MsgContent = oOutput.serialize();
	iLength = MsgContent.size();

	auto pret = new std::string();

	pret->push_back((iLength >> 0) & 0xff);
	pret->push_back((iLength >> 8) & 0xff);
	pret->push_back((iLength >> 16) & 0xff);
	pret->push_back((iLength >> 24) & 0xff);
	pret->push_back((id >> 0) & 0xff);
	pret->push_back((id >> 8) & 0xff);
	pret->push_back((id >> 16) & 0xff);
	pret->push_back((id >> 24) & 0xff);
	pret->append(MsgContent);

	return pret;
}

Irole* GameProtocol::GetMsgProcessor(UserDataMsg& _oUserDataMsg)
{
	return m_Role;
}

/*返回数据发送的通道*/
Ichannel* GameProtocol::GetMsgSender(BytesMsg& _oBytes)
{
	return m_channel;
}
