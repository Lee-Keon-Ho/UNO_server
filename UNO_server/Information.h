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

	struct LIST
	{
		RoomList roomlist;
		UserList userlist;
	};

private:
	LIST m_list;

public:
	LIST* GetList() { return &m_list; }

public:
	static CInformation* GetInstance();
	static void DeleteInstance();
};