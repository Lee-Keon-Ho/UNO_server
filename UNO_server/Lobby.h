#pragma once
#include "Room.h"
#include "user.h"
#include <list>
#include <vector>

class CLobby // �ϳ��� ä���̴� ��� ��������
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