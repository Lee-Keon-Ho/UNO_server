#pragma once
#include "Room.h"
#include <vector>

// 2022-04-25 ����
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
	typedef std::vector<CRoom*> roomList_t; // ȣ���̴�

private:
	roomList_t m_roomList;
	int m_count;

public:
	bool Initialize();
	void Cleanup();

	CRoom* CreateRoom(char* _name);
	CRoom* RoomIn(char* _playerInfo, SOCKET _socket);
	void RoomOut();

	int GetCount() { return m_count; }
	roomList_t* GetRoomList() { return &m_roomList; }
};