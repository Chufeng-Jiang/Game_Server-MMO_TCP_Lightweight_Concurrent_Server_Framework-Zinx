Zinx框架-游戏服务器开发002：按照三层结构模式重构代码功能-待续

@[TOC]

# 1 Zinx框架总览

<img src="https://img-blog.csdnimg.cn/bba5190d8e6d49b6a46db57dbe4a7264.png" title="" alt="在这里插入图片描述" data-align="inline">

# 2 三层模式的分析

![在这里插入图片描述](https://img-blog.csdnimg.cn/8c23e7f61cd94ecda1655ea8ddfdab3e.png)

# 3 三层重构原有的功能 - 头文件

三层结构重构原有功能

1. 自定义消息类，继承UserData，添加一个成员变量szUserData
2. 定义多个Role类继承Irole，重写ProcMsg函数，进行不同处理
3. 定义protocol类，继承Iprotocol，重写四个函数，两个函数时原始
   数据和用户数据之间的转换；另两个用来找消息处理对象和消息发
   送对象。
4. 定义channel类，继承Ichannel，在getnextinputstage函数中返回协
   议对象

## 3.1 通道层Stdin和Stdout类

通道类，派生自基础处理者类，提供基于系统调用的数据收发功能
一般地，用户应该根据处理的文件（信息源）不同而创建通道类的子类或选用合适的实用类（已经提供的通道类子类）来完成系统级文件IO
![在这里插入图片描述](https://img-blog.csdnimg.cn/0fb4b736a4e447a69331786c7902b19b.png)

```c
class StdInChannel :
    public Ichannel
{
public:
    StdInChannel();
    virtual ~StdInChannel();

    // 通过 Ichannel 继承
    virtual bool Init() override;
    virtual bool ReadFd(std::string& _input) override;
    virtual bool WriteFd(std::string& _output) override;
    virtual void Fini() override;
    virtual int GetFd() override;
    virtual std::string GetChannelInfo() override;
    virtual AZinxHandler* GetInputNextStage(BytesMsg& _oInput) override;
};

class StdOutChannel :public Ichannel
{
    // 通过 Ichannel 继承
    virtual bool Init() override;
    virtual bool ReadFd(std::string& _input) override;
    virtual bool WriteFd(std::string& _output) override;
    virtual void Fini() override;
    virtual int GetFd() override;
    virtual std::string GetChannelInfo() override;
    virtual AZinxHandler* GetInputNextStage(BytesMsg& _oInput) override;
};
```

### 3.1.2 StdInChannel

```cpp
bool StdInChannel::ReadFd(std::string& _input)
{
    cin >> _input;
    return true;
}

bool StdInChannel::WriteFd(std::string& _output)
{
    return false;
}

int StdInChannel::GetFd()
{
    return 0;
}

std::string StdInChannel::GetChannelInfo()
{
    return "stdin";
}

AZinxHandler* StdInChannel::GetInputNextStage(BytesMsg& _oInput)
{
    /*返回协议对象*/
    return CmdCheck::GetInstance();
}
```

### 3.1.2 StdOutChannel

```c
bool StdOutChannel::ReadFd(std::string& _input)
{
    return false;
}

bool StdOutChannel::WriteFd(std::string& _output)
{
    cout << _output << endl;
    return true;
}

int StdOutChannel::GetFd()
{
    return 1;
}

std::string StdOutChannel::GetChannelInfo()
{
    return "stdout";
}

AZinxHandler* StdOutChannel::GetInputNextStage(BytesMsg& _oInput)
{
    return nullptr;
}
```

## 3.2 协议层CmdCheck和CmdMsg类

### 3.2.1 CmdCheck单例模式

1. 原始数据和业务数据相互函数，开发者重写该函数，实现协议
2. 获取处理角色对象函数，开发者应该重写该函数，用来指定当前产生的用户数据消
3. 获取发送通道函数，开发者应该重写该函数，用来指定当前字节流应该由哪个通道对象发出

```csharp
class CmdCheck :
    public Iprotocol
{
    CmdCheck();
    virtual ~CmdCheck();
    static CmdCheck *poSingle;
public:
    // 通过 Iprotocol 继承
    /*原始数据和业务数据相互函数，开发者重写该函数，实现协议*/
    virtual UserData * raw2request(std::string _szInput) override;
    virtual std::string * response2raw(UserData & _oUserData) override;
    /*获取处理角色对象函数，开发者应该重写该函数，用来指定当前产生的用户数据消息应该传递给哪个角色处理*/
    virtual Irole * GetMsgProcessor(UserDataMsg & _oUserDataMsg) override;
    /*获取发送通道函数，开发者应该重写该函数，用来指定当前字节流应该由哪个通道对象发出*/
    virtual Ichannel * GetMsgSender(BytesMsg & _oBytes) override;
    static CmdCheck *GetInstance() {
        return poSingle;
    }
    std::string szOutChannel;
};
```

#### 3.2.1.1 单例模式

构造全局唯一的协议对象

```c
#include "CmdCheck.h"
#include "CmdMsg.h"
#include "EchoRole.h"
using namespace std;

CmdCheck *CmdCheck::poSingle = new CmdCheck();
```

#### 3.2.1.2 * 命令识别类向业务层不同类别做分发

通过是不是命令来进行区分：`if (isCmd)`

```c
Irole * CmdCheck::GetMsgProcessor(UserDataMsg & _oUserDataMsg)
{
    szOutChannel = _oUserDataMsg.szInfo;
    if ("stdin" == szOutChannel)
    {
        szOutChannel = "stdout";
    }
    /*根据命令不同，交给不同的处理role对象*/
    auto rolelist = ZinxKernel::Zinx_GetAllRole();

    auto pCmdMsg = dynamic_cast<CmdMsg *>(_oUserDataMsg.poUserData);

    /*读取当前消息是否是命令*/
    bool isCmd = pCmdMsg->isCmd;

    Irole *pRetRole = NULL;

    for (Irole *prole : rolelist)
    {
        if (isCmd)
        {
            auto pOutCtrl = dynamic_cast<OutputCtrl *>(prole);
            if (NULL != pOutCtrl)
            {
                pRetRole = pOutCtrl;
                break;
            }
        }
        else
        {
            auto pDate = dynamic_cast<DatePreRole *>(prole);
            if (NULL != pDate)
            {
                pRetRole = pDate;
                break;
            }
        }
    }

    return pRetRole;
}
```

### 3.2.2 CmdMsg自定义用户信息类，继承UserData

```cpp
class CmdMsg :
    public UserData
{
public:
    /*成员变量表示要回显的字符串*/
    std::string szUserData;
    /*开启输出标志*/
    bool isOpen = true;
    /*该消息是命令*/
    bool isCmd = false;
    /*要加前缀*/
    bool needDatePre = false;

    CmdMsg();
    virtual ~CmdMsg();
};
```

## 3.3 业务层：回显类, 输出通道控制类, 日期前缀管理类

### 3.3.1 回显对象EchoRole

主要有init， procmsg，fini三个函数

```cpp
#pragma once
#include <zinx.h>

class EchoRole :
    public Irole
{
public:
    EchoRole();
    virtual ~EchoRole();

    // 通过 Irole 继承
    virtual bool Init() override;
    virtual UserData * ProcMsg(UserData & _poUserData) override;
    virtual void Fini() override;
};
```

- 容易出错的点：参数一必须是一个堆对象

```cpp
UserData * EchoRole::ProcMsg(UserData & _poUserData)
{
    /*写出去*/
    GET_REF2DATA(CmdMsg, input, _poUserData);
    CmdMsg *pout = new CmdMsg(input);
    ZinxKernel::Zinx_SendOut(*pout, *(CmdCheck::GetInstance()));
    return nullptr;
}
```

### 3.3.2 控制输入输出

> 1. 写一个关闭输出的角色类，摘除输出通道或添加输出通道
> 2. 在CmdMsg用户数据类中添加开关标志，是否是命令标志
> 3. 在协议类中，根据输入字符串，设置开关标志和是否是命令的标志
> 4. 在协议类分发消息时，判断是否是命令，是命令则发给关闭输出角 色类，否则发给回显角色类

```c
class OutputCtrl :public Irole {
    // 通过 Irole 继承
    virtual bool Init() override;
    virtual UserData * ProcMsg(UserData & _poUserData) override;
    virtual void Fini() override;
    Ichannel *pOut = NULL;
};
```

### 3.3.3 日期管理类

```cpp
class DatePreRole :public Irole {
    // 通过 Irole 继承
    virtual bool Init() override;
    virtual UserData * ProcMsg(UserData & _poUserData) override;
    virtual void Fini() override;
    bool needAdd = false;
};
```

## 3.4 Tcp数据适配

### 3.4.1 工厂类 - 框架头文件分析

- 产生tcp数据套接字通道类的抽象工厂类。
  
  - 开发者需要重写CreateTcpDataChannel函数，来返回一个tcp通道对象。
  
  - 般地，开发者应该同时创建一对tcp通道类和工厂类
    
    ```c
    class IZinxTcpConnFact {
    public:
    virtual ZinxTcpData *CreateTcpDataChannel(int _fd) = 0;
    };
    ```
    
    - tcp监听通道类，这是一个实体类（不建议继承该类）。
    - 开发者可以直接创建tcp监听通道对象，
    - 一般地，开发者应该在该类的构造函数中，指定一个tcp套接字通道类的工厂类，当有连接到来后，该工厂类的成员方法会被调用

```c
class ZinxTCPListen :
    public Ichannel
{
private:
    unsigned short m_usPort = 0;
    int m_fd = -1;
    IZinxTcpConnFact *m_ConnFac = NULL;

public:
    ZinxTCPListen(unsigned short _usPort, IZinxTcpConnFact *_pConnFac) :m_usPort(_usPort), m_ConnFac(_pConnFac){}
    virtual ~ZinxTCPListen();

    virtual bool Init() override;
    virtual bool ReadFd(std::string & _input) override;
    virtual bool WriteFd(std::string & _output) override;
    virtual void Fini() override;
    virtual int GetFd() override;
    virtual std::string GetChannelInfo() override;
    virtual AZinxHandler * GetInputNextStage(BytesMsg & _oInput);
};
```

### 3.4.2 tcp通道实现

![在这里插入图片描述](https://img-blog.csdnimg.cn/df4e807ad47f4aa3be6d8f418b469b8e.png)

#### 3.4.2.1 Tcp套接字通道通信类

- tcp数据套接字通道类，继承通道类，该类也是一个抽象类，需要开发者继承该类，重写GetInputNextStage函数以指定读取到的字节流的处理方式

```c
// h
class myTcpData :public ZinxTcpData {
public:
    myTcpData(int _fd) :ZinxTcpData(_fd) {}
    // 通过 ZinxTcpData 继承
    virtual AZinxHandler* GetInputNextStage(BytesMsg& _oInput) override;
};
```

```cpp
AZinxHandler* myTcpData::GetInputNextStage(BytesMsg& _oInput)
{
    /*返回协议对象*/
    return CmdCheck::GetInstance();
}
```

#### 3.4.2.2 tcp数据套接字通道类的工厂类

- 产生tcp数据套接字通道类的抽象工厂类，开发者需要重写CreateTcpDataChannel函数，来返回一个tcp通道对象
  一般地，开发者应该同时创建一对tcp通道类和工厂类

```c
// h
class myFact :public IZinxTcpConnFact {
    // 通过 IZinxTcpConnFact 继承
    virtual ZinxTcpData* CreateTcpDataChannel(int _fd) override;
};
```

```c
ZinxTcpData* myFact::CreateTcpDataChannel(int _fd)
{
    return new myTcpData(_fd);
}
```
