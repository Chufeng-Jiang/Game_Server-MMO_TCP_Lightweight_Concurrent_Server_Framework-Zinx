#include "Ichannel.h"
#include "zinxkernel.h"


Ichannel::Ichannel()
{
}


Ichannel::~Ichannel()
{
}

void Ichannel::flushout()
{
	WriteFd(m_buffer);
	m_buffer.clear();
}

//输出数据--》将参数记录到缓存
void Ichannel::data_sendout(std::string _output)
{
	m_buffer.append(_output.begin(), _output.end());
	//修改epoll监听类型--》添加out方向的监听
	zinxkernel::GetInstance().ModChannel_AddOut(this);
}
