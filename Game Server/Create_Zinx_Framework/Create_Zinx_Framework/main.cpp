#include <iostream>
#include "zinxkernel.h"
#include "stdin_channel.h"
#include "FIFO_channel.h"

using namespace std;

int main()
{
	stdin_channel in_channel;
	stdou_channel out_channel;
	in_channel.m_out = &out_channel;

	zinxkernel &kernel = zinxkernel::GetInstance();
	kernel.AddChannel(&in_channel);
	kernel.AddChannel(&out_channel);

	FIFO_channel fifo_input("input", true);// 创建管道input
	FIFO_channel fifo_output("output", false); //创建管道output
	fifo_input.m_out = &fifo_output;
	kernel.AddChannel(&fifo_input);
	kernel.AddChannel(&fifo_output);

	kernel.run();
}