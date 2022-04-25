#pragma once
#include "Room.h"
#include <vector>

// 2022-04-25 수정
class CRoomManager
{
public:
	typedef std::vector<CRoom*> roomList_t; // 호텔이다

private:
	roomList_t m_roomList;
	int m_count;
public:
	CRoomManager();
	~CRoomManager();

	CRoom* CreateRoom(char* _buffer);
	void ResetRoom(int _num);
	void OutRoom(int _num);

	int GetCount() { return m_count; }

	roomList_t* GetRoomList() { return &m_roomList; }
};