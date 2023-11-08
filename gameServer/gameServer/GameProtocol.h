#pragma once
#include <zinx.h>

class GameChannel;
class GameRole;
class GameProtocol :
	public Iprotocol
{
	std::string szLast;
public:
	GameChannel *m_channel = NULL;
	GameRole *m_Role = NULL;
	GameProtocol();
	virtual ~GameProtocol();

	// Í¨¹ý Iprotocol ¼Ì³Ð
	virtual UserData * raw2request(std::string _szInput) override;
	virtual std::string * response2raw(UserData & _oUserData) override;
	virtual Irole * GetMsgProcessor(UserDataMsg & _oUserDataMsg) override;
	virtual Ichannel * GetMsgSender(BytesMsg & _oBytes) override;
};

