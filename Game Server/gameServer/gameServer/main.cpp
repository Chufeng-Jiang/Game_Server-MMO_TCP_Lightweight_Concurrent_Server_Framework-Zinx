#include "GameChannel.h"
#include "GameMsg.h"
#include "msg.pb.h"
#include "AOIWorld.h"

class myPlayer :public Player {
public:
	myPlayer(int _x, int _y, std::string _name) :x(_x), y(_y), name(_name) {}
	int x;
	int y;
	std::string name;
	// 通过 Player 继承
	virtual int GetX() override
	{
		return x;
	}
	virtual int GetY() override
	{
		return y;
	}
};

int main()
{
	AOIWorld w(20, 200, 50, 230, 6, 6);
	myPlayer p1(60, 107, "1");
	myPlayer p2(91, 118, "2");
	myPlayer p3(147, 133, "3");
	w.AddPlayer(&p1);
	w.AddPlayer(&p2);
	w.AddPlayer(&p3);

	auto srd_list = w.GetSrdPlayers(&p1);
	for (auto single : srd_list)
	{
		std::cout << dynamic_cast<myPlayer*>(single)->name << std::endl;
	}

	ZinxKernel::ZinxKernelInit();
	/*添加监听通道*/
	ZinxKernel::Zinx_Add_Channel(*(new ZinxTCPListen(8899, new GameConnFact())));
	ZinxKernel::Zinx_Run();
	ZinxKernel::ZinxKernelFini();
}