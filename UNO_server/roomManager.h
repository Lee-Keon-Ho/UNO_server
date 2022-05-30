#pragma once
#include "Room.h"
#include <vector>

class CRoomManager
{
private:
	static CRoomManager* pInstance;
	CRoomManager();
	~CRoomManager();

public:
	static CRoomManager* GetInstance();
	static void DeleteInstance();

public:
	typedef std::vector<CRoom*> roomList_t; // 호텔이다

private:
	roomList_t m_roomList;
	int m_count;

public:
	bool Initialize();
	void Cleanup();

	CRoom* CreateRoom(char* _name);
	CRoom* RoomIn(int _roomNumber);
	CRoom* QuickRoomIn();
	void RoomOut();

	int GetCount() { return m_count; }
	roomList_t* GetRoomList() { return &m_roomList; }
};