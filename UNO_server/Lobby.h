#pragma once
#include "Room.h"
#include "user.h"
#include <list>
#include <vector>

// 2022-05-03 수정 : TEST
class CLobby // 하나의 채널이다 라고 생각하자
{
public:
	typedef std::vector<CRoom*> roomList_t; // class
	typedef std::list<CUser*> userList_t; // class

private:
	roomList_t m_roomList;
	userList_t m_userList;
	// addUser
	// roomIn
public:
	CLobby();
	~CLobby();

	void AddUser(CUser* _user);
};