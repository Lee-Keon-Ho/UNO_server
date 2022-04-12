#pragma once
#include "Room.h"
#include <list>

/*create 2022 - 04 - 12
	room만 관리하기 위해서
*/
class CRoomManager
{
private:
	static CRoomManager* pInstance;
	CRoomManager();
	~CRoomManager();

public:
	typedef std::list<CRoom*> roomList_t;

private:
	roomList_t m_roomList;

public:
	roomList_t* GetRoomList() { return &m_roomList; }
	void Remove(CRoom* _room);

public:
	static CRoomManager* GetInstance();
	static void DeleteInstance();
};