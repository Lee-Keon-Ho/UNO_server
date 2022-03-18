#include "UserList.h"

#define MAX 64

CUserList::CUserList()
{
	m_userList.reserve(MAX);
}

CUserList::~CUserList()
{
}

void CUserList::Add(char* _recvBuffer)
{
	m_userList.push_back(new CUser(_recvBuffer));
}

void CUserList::Remove()
{
}
