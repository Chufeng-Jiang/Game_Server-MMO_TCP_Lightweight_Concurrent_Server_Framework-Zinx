#include "ProcessFunc.h"
#include <algorithm>
#include "zinxkernel.h"

using namespace std;

ProcessFunc::ProcessFunc()
{
}


ProcessFunc::~ProcessFunc()
{
}

ZinxMsg * ProcessFunc::internel_handle(ZinxMsg * _inputMsg)
{
	auto byte = dynamic_cast<ByteMsg *>(_inputMsg);
	if (NULL != byte)
	{
		std::string NextString = byte->content;
		if (NextString[0] <= 'z' && NextString[0] >= 'a')
		{
			transform(NextString.begin(), NextString.end(), NextString.begin(), ::toupper);
		}
		
		//调用发送函数
		zinxkernel::zin_sendout(NextString, m_out);
	}
	
	return nullptr;
}

ZinxHandler * ProcessFunc::GetNext(ZinxMsg * _next_input)
{
	return NULL;
}
