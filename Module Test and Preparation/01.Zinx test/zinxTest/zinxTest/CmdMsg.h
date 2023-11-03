#pragma once
#include <zinx.h>
#include <string>

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

