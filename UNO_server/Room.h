#pragma once

// �� �ϳ�
class CRoom
{
public:
	struct stROOM
	{
		int number;
		char name[64];
		int playerCount;
		int playerMax;
		bool state;
	};
private:
	stROOM m_room;

public:
	CRoom();
	CRoom(int _number, char* _name);
	~CRoom();
private:
};