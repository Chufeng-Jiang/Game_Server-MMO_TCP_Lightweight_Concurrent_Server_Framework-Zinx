#include "GameRole.h"
#include "GameMsg.h"
#include <iostream>
#include "AOIWorld.h"
#include "msg.pb.h"
#include "GameProtocol.h"
#include "GameChannel.h"
#include <algorithm>
#include <random>
#include "ZinxTimer.h"
#include "RandomName.h"

using namespace std;

RandomName random_name;

/*创建游戏世界全局对象*/
static AOIWorld world(0, 400, 0, 400, 20, 20);

void GameRole::ProcTalkMsg(std::string _content)
{
	//发给所有人
	auto role_list = ZinxKernel::Zinx_GetAllRole();
	for (auto pRole : role_list)
	{
		auto pGameRole = dynamic_cast<GameRole *>(pRole);
		auto pmsg = CreateTalkBroadCast(_content);
		ZinxKernel::Zinx_SendOut(*pmsg, *(pGameRole->m_pProto));
	}
}

void GameRole::ProcMoveMsg(float _x, float _y, float _z, float _v)
{
	/*1.跨网格处理*/
	/*获取原来的邻居s1*/
	auto s1 = world.GetSrdPlayers(this);
	/*摘出旧格子,更新坐标,添加新格子，获取新邻居s2*/
	world.DelPlayer(this);
	x = _x;
	y = _y;
	z = _z;
	v = _v;
	world.AddPlayer(this);
	auto s2 = world.GetSrdPlayers(this);
	/*遍历s2，若元素不属于s1, 视野出现*/
	for (auto single_player : s2)
	{
		if (s1.end() == find(s1.begin(), s1.end(), single_player))
		{
			//视野出现
			ViewAppear(dynamic_cast<GameRole *>(single_player));
		}
	}
	/*遍历s1，若元素不属于s2，视野消失*/
	for (auto single_player : s1)
	{
		if (s2.end() == find(s2.begin(), s2.end(), single_player))
		{
			//视野消失
			ViewLost(dynamic_cast<GameRole *>(single_player));
		}
	}

	/*2.广播新位置给周围玩家*/
	
	//遍历周围玩家发送
	/*向周围玩家发送自己的位置*/
	auto srd_list = world.GetSrdPlayers(this);
	for (auto single : srd_list)
	{
		//组成待发送的报文
		pb::BroadCast *pMsg = new pb::BroadCast();
		auto pPos = pMsg->mutable_p();
		pPos->set_x(_x);
		pPos->set_y(_y);
		pPos->set_z(_z);
		pPos->set_v(_v);
		pMsg->set_pid(iPid);
		pMsg->set_tp(4);
		pMsg->set_username(szName);
		auto pRole = dynamic_cast<GameRole *>(single);
		ZinxKernel::Zinx_SendOut(*(new GameMsg(GameMsg::MSG_TYPE_BROADCAST, pMsg)), *(pRole->m_pProto));
	}
}

void GameRole::ViewAppear(GameRole * _pRole)
{
	/*向自己发参数的200消息*/
	auto pmsg = _pRole->CreateSelfPostion();
	ZinxKernel::Zinx_SendOut(*pmsg, *m_pProto);

	/*向参数玩家发自己的200消息*/
	pmsg = CreateSelfPostion();
	ZinxKernel::Zinx_SendOut(*pmsg, *(_pRole->m_pProto));

}

void GameRole::ViewLost(GameRole * _pRole)
{
	/*向自己发送参数玩家的201消息*/
	auto pmsg = _pRole->CreateIDNameLogoff();
	ZinxKernel::Zinx_SendOut(*pmsg, *m_pProto);
	/*向参数玩家发自己的201消息*/
	pmsg = CreateIDNameLogoff();
	ZinxKernel::Zinx_SendOut(*pmsg, *(_pRole->m_pProto));
}

GameMsg * GameRole::CreateIDNameLogin()
{
	pb::SyncPid *pmsg = new pb::SyncPid();
	pmsg->set_pid(iPid);
	pmsg->set_username(szName);
	GameMsg *pRet = new GameMsg(GameMsg::MSG_TYPE_LOGIN_ID_NAME, pmsg);
	return pRet;
}

GameMsg * GameRole::CreataSrdPlayers()
{
	pb::SyncPlayers *pMsg = new pb::SyncPlayers();

	auto srd_list = world.GetSrdPlayers(this);

	for (auto single : srd_list)
	{
		auto pPlayer = pMsg->add_ps();
		auto pRole = dynamic_cast<GameRole *>(single);
		pPlayer->set_pid(pRole->iPid);
		pPlayer->set_username(pRole->szName);
		auto pPostion = pPlayer->mutable_p();
		pPostion->set_x(pRole->x);
		pPostion->set_y(pRole->y);
		pPostion->set_z(pRole->z);
		pPostion->set_v(pRole->v);
	}

	GameMsg *pret = new GameMsg(GameMsg::MSG_TYPE_SRD_POSTION, pMsg);
	return pret;
}

GameMsg * GameRole::CreateSelfPostion()
{
	pb::BroadCast *pMsg = new pb::BroadCast();
	pMsg->set_pid(iPid);
	pMsg->set_username(szName);
	pMsg->set_tp(2);

	auto pPosition = pMsg->mutable_p();
	pPosition->set_x(x);
	pPosition->set_y(y);
	pPosition->set_z(z);
	pPosition->set_v(v);

	GameMsg *pret = new GameMsg(GameMsg::MSG_TYPE_BROADCAST, pMsg);
	return pret;
}

GameMsg * GameRole::CreateIDNameLogoff()
{
	pb::SyncPid *pmsg = new pb::SyncPid();
	pmsg->set_pid(iPid);
	pmsg->set_username(szName);
	GameMsg *pRet = new GameMsg(GameMsg::MSG_TYPE_LOGOFF_ID_NAME, pmsg);
	return pRet;
}

GameMsg * GameRole::CreateTalkBroadCast(std::string _content)
{
	pb::BroadCast *pmsg = new pb::BroadCast();
	pmsg->set_pid(iPid);
	pmsg->set_username(szName);
	pmsg->set_tp(1);
	pmsg->set_content(_content);
	GameMsg *pRet = new GameMsg(GameMsg::MSG_TYPE_BROADCAST, pmsg);
	return pRet;
}

static default_random_engine random_engine(time(NULL));

GameRole::GameRole()
{
	szName = random_name.GetName();
	x = 100 + random_engine() % 50;
	z = 100 + random_engine() % 50;
}


GameRole::~GameRole()
{
	random_name.Release(szName);
}
class ExitTimer :public TimerOutProc {
	// 通过 TimerOutProc 继承
	virtual void Proc() override
	{
		ZinxKernel::Zinx_Exit();
	}
	virtual int GetTimeSec() override
	{
		return 20;
	}
};
static ExitTimer g_exit_timer;
bool GameRole::Init()
{
	if (ZinxKernel::Zinx_GetAllRole().size() <= 0)
	{
		TimerOutMng::GetInstance().DelTask(&g_exit_timer);
	}

	/*添加自己到游戏世界*/
	bool bRet = false;
	/*设置玩家ID为当前连接的fd*/
	iPid = m_pProto->m_channel->GetFd();
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
			auto pRole = dynamic_cast<GameRole *>(single);
			ZinxKernel::Zinx_SendOut(*pmsg, *(pRole->m_pProto));
		}
	}

	return bRet;
}

/*处理游戏相关的用户请求*/
UserData * GameRole::ProcMsg(UserData & _poUserData)
{
	GET_REF2DATA(MultiMsg, input, _poUserData);

	for (auto single : input.m_Msgs)
	{
		/*测试：打印消息内容*/
		cout << "type is" << single->enMsgType << endl;
		cout << single->pMsg->Utf8DebugString() << endl;

		auto NewPos = dynamic_cast<pb::Position *>(single->pMsg);
		switch (single->enMsgType)
		{
		case GameMsg::MSG_TYPE_CHAT_CONTENT:
			ProcTalkMsg(dynamic_cast<pb::Talk *>(single->pMsg)->content());
			break;
		case GameMsg::MSG_TYPE_NEW_POSTION:
			ProcMoveMsg(NewPos->x(), NewPos->y(),NewPos->z(),NewPos->v());
			break;
		default:
			break;
		}
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
		auto pRole = dynamic_cast<GameRole *>(single);
		ZinxKernel::Zinx_SendOut(*pMsg, *(pRole->m_pProto));
	}
	world.DelPlayer(this);

	/*判断是否是最后一个玩家--->起定时器*/
	if (ZinxKernel::Zinx_GetAllRole().size() <= 1)
	{
		//起退出定时器
		TimerOutMng::GetInstance().AddTask(&g_exit_timer);
	}
}

int GameRole::GetX()
{
	return (int)x;
}

int GameRole::GetY()
{
	return (int)z;
}
