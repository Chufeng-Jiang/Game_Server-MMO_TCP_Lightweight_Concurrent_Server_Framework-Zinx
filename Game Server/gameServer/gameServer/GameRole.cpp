#include "GameRole.h"
#include "GameMsg.h"
#include <iostream>
#include "msg.pb.h"
#include "GameProtocol.h"
#include "GameChannel.h"
using namespace std;

GameRole::GameRole()
{
}

GameRole::~GameRole()
{
}

bool GameRole::Init()
{
	return true;
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
}

