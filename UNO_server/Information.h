#pragma once
#include "user.h"
#include "Room.h"
#include <list>

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
	
	void RemoveUser(CUser* _user);
	void RemoveRoom(CRoom* _room);
public:
	static CInformation* GetInstance();
	static void DeleteInstance();
};