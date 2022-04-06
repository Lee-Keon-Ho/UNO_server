#pragma once

#define NAME_MAX 16

class CUser
{
public:

private:
	char m_pName[NAME_MAX];
public:
	CUser();
	~CUser();
	CUser(char* _recvBuffer);

	char* GetName() { return m_pName; }
};