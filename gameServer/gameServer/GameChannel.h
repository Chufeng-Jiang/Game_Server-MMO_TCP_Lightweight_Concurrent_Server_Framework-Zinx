#pragma once
#include<ZinxTCP.h>
#include "GameProtocol.h"

class GameChannel :
	public ZinxTcpData
{
public:
	GameChannel(int _fd);
	virtual ~GameChannel();
	GameProtocol *m_proto = NULL;
	// ����Э�����
	virtual AZinxHandler * GetInputNextStage(BytesMsg & _oInput) override;
};

class GameConnFact :public IZinxTcpConnFact {
	// ͨ�� IZinxTcpConnFact �̳�
	virtual ZinxTcpData * CreateTcpDataChannel(int _fd) override;
};

