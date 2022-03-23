#pragma once

// 방 하나
class CRoom
{
public:
	struct Room
	{
		int number;
		char name[64];
		int playerCount;
		bool state;
	};
private:
	// 방이름 (만든사람이름으로 사용할 생각)
	// 방 번호
	// 게임 진행 상황
	Room m_room;
public:
	CRoom();
	CRoom(char* _name);
	~CRoom();
private:
};