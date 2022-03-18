#pragma once

// 방 하나
class CRoom
{
public:
private:
	// 방이름 (만든사람이름으로 사용할 생각)
	// 방 번호
	// 게임 진행 상황
	char* m_roomName;
	char* m_roomNum;
	char* m_state;
public:
	CRoom();
	~CRoom();
private:
};