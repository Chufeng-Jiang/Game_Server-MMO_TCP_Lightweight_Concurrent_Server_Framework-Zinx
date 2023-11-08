#pragma once
#include <list>
#include <string>
#include <vector>

//�պ�    ����ɵ����Ա�
class FirstName {
public:
	std::string m_first;
	std::vector<std::string> m_last_list;
};

class RandomName
{
	std::vector<FirstName *> m_pool;
public:
	RandomName();
	std::string GetName();
	void Release(std::string _name);
	void LoadFile();
	virtual ~RandomName();
};

