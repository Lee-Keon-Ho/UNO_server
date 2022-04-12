#pragma once
#include "user.h"
#include <list>

/* create 2022 - 04 - 12
	user만 관리하기 위해서
*/
class CUserManager
{
private:
	static CUserManager* pInstance;
	CUserManager();
	~CUserManager();

public:
	typedef std::list<CUser*> userList_t;

private:
	userList_t m_userList;

public:
	userList_t* GetUserList() { return &m_userList; }
	void Remove(CUser* _user);

public:
	static CUserManager* GetInstance();
	static void DeleteInstance();
};