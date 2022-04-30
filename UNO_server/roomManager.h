#pragma once
#include "Room.h"
#include <vector>

// 2022-04-25 수정
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
	CRoom* InRoom(char* _playerInfo, SOCKET _socket);
	void OutRoom(int _num);

	int GetCount() { return m_count; }
	roomList_t* GetRoomList() { return &m_roomList; }
};