#include "GameRole.h"
#include "GameMsg.h"
#include <iostream>
#include "AOIWorld.h"
#include "msg.pb.h"
#include "GameProtocol.h"
#include "GameChannel.h"

using namespace std;

/*创建游戏世界全局对象*/
static AOIWorld world(0, 400, 0, 400, 20, 20);

GameMsg* GameRole::CreateIDNameLogin()
{
	pb::SyncPid* pmsg = new pb::SyncPid();
	pmsg->set_pid(iPid);
	pmsg->set_username(szName);
	GameMsg* pRet = new GameMsg(GameMsg::MSG_TYPE_LOGIN_ID_NAME, pmsg);
	return pRet;
}

GameMsg* GameRole::CreataSrdPlayers()
{
	pb::SyncPlayers* pMsg = new pb::SyncPlayers();

	auto srd_list = world.GetSrdPlayers(this);

	//周围玩家有多个
	for (auto single : srd_list)
	{
		auto pPlayer = pMsg->add_ps();
		auto pRole = dynamic_cast<GameRole*>(single);
		//设置到遍历到的玩家的信息
		pPlayer->set_pid(pRole->iPid);
		pPlayer->set_username(pRole->szName);

		//把子消息挂到父消息里面，并返回子消息的指针
		auto pPostion = pPlayer->mutable_p();
		pPostion->set_x(pRole->x);
		pPostion->set_y(pRole->y);
		pPostion->set_z(pRole->z);
		pPostion->set_v(pRole->v);
	}

	GameMsg* pret = new GameMsg(GameMsg::MSG_TYPE_SRD_POSTION, pMsg);
	return pret;
}

GameMsg* GameRole::CreateSelfPostion()
{
	pb::BroadCast* pMsg = new pb::BroadCast();
	pMsg->set_pid(iPid);
	pMsg->set_username(szName);
	pMsg->set_tp(2); //客户端决定的要设置成2

	auto pPosition = pMsg->mutable_p();
	pPosition->set_x(x);
	pPosition->set_y(y);
	pPosition->set_z(z);
	pPosition->set_v(v);

	GameMsg* pret = new GameMsg(GameMsg::MSG_TYPE_BROADCAST, pMsg);
	return pret;
}

GameMsg* GameRole::CreateIDNameLogoff()
{
	pb::SyncPid* pmsg = new pb::SyncPid();
	pmsg->set_pid(iPid);
	pmsg->set_username(szName);
	GameMsg* pRet = new GameMsg(GameMsg::MSG_TYPE_LOGOFF_ID_NAME, pmsg);
	return pRet;
}

GameRole::GameRole()
{
	szName = "Tom";
	x = 100;
	z = 100;
}


GameRole::~GameRole()
{

}

bool GameRole::Init()
{
	/*添加自己到游戏世界*/
	bool bRet = false;
	/*设置玩家ID为当前连接的fd*/
	iPid = m_pProto->m_channel->GetFd(); //获取文件连接描述符，这个是唯一的
	bRet = world.AddPlayer(this);


	if (true == bRet)
	{
		/*向自己发送ID和名称*/
		auto pmsg = CreateIDNameLogin();
		ZinxKernel::Zinx_SendOut(*pmsg, *m_pProto);
		/*向自己发送周围玩家的位置*/
		pmsg = CreataSrdPlayers();
		ZinxKernel::Zinx_SendOut(*pmsg, *m_pProto);
		/*向周围玩家发送自己的位置*/
		auto srd_list = world.GetSrdPlayers(this);
		for (auto single : srd_list)
		{
			pmsg = CreateSelfPostion();
			auto pRole = dynamic_cast<GameRole*>(single);

			// 注意第二个参数
			ZinxKernel::Zinx_SendOut(*pmsg, *(pRole->m_pProto));
		}
	}

	return bRet;
}

/*处理游戏相关的用户请求*/
UserData* GameRole::ProcMsg(UserData& _poUserData)
{
	/*测试：打印消息内容*/
	GET_REF2DATA(MultiMsg, input, _poUserData);

	for (auto single : input.m_Msgs)
	{
		cout << "type is" << single->enMsgType << endl;
		cout << single->pMsg->Utf8DebugString() << endl;
	}

	return nullptr;
}

void GameRole::Fini()
{
	/*向周围玩家发送下线消息*/
	auto srd_list = world.GetSrdPlayers(this);
	for (auto single : srd_list)
	{
		auto pMsg = CreateIDNameLogoff();
		auto pRole = dynamic_cast<GameRole*>(single);
		ZinxKernel::Zinx_SendOut(*pMsg, *(pRole->m_pProto));
	}
	world.DelPlayer(this);
}

int GameRole::GetX()
{
	return (int)x;
}

int GameRole::GetY()
{
	return (int)z;
}
