#pragma once
#include "list.h"
#include "User.h"
#include <vector>
class CUserList : public CList
{
public:
	typedef std::vector<CUser*> UserList;
private:
	UserList m_userList;
public:
	CUserList();
	~CUserList();

	void Add(char* _recvBuffer);
	void Remove();
private:
};