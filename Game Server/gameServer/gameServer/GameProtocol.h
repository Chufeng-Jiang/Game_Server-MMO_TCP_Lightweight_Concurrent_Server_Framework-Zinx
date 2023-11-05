#pragma once
#include <zinx.h>

class GameChannel;  //避免循环引用

class GameProtocol :
    public Iprotocol
{
    std::string szLast; //上次未来得及处理的报文
public:
    GameChannel* m_channel = NULL;
    GameProtocol() ;
    virtual ~GameProtocol();

    // 通过 Iprotocol 继承
    virtual UserData* raw2request(std::string _szInput) override;
    virtual std::string* response2raw(UserData& _oUserData) override;
    virtual Irole* GetMsgProcessor(UserDataMsg& _oUserDataMsg) override;
    virtual Ichannel* GetMsgSender(BytesMsg& _oBytes) override;
};

