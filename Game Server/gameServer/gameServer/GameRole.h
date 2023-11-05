#pragma once
#include "zinx.h"

class GameProtocol;

class GameRole :
    public Irole
{
public:
    GameRole() ;
    virtual ~GameRole();

    // 通过 Irole 继承
    virtual bool Init() override;
    virtual UserData* ProcMsg(UserData& _poUserData) override;
    virtual void Fini() override;

    GameProtocol* m_pProto = NULL;
};

