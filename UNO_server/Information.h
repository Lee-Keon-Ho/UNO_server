#pragma once
#include "user.h"
#include "Room.h"
#include "session.h"
#include <list>


// 수정 : sessionManager 각각 자신이 관리하는 하나만 manager로 
//        roomManager

class CInformation
{
private:
	static CInformation* pInstance;
	CInformation();
	~CInformation();

public:
	typedef std::list<CRoom*> RoomList;
	typedef std::list<CUser*> UserList;

private:
	RoomList m_roomList;
	UserList m_userList;

public:
	UserList* GetUserList() { return &m_userList; }
	RoomList* GetRoomList() { return &m_roomList; }
	
	void RemoveUser(CSession* _session);
	void RemoveRoom(CSession* _session);
public:
	static CInformation* GetInstance();
	static void DeleteInstance();
};