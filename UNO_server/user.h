#pragma once
#include <wchar.h>

#define NAME_MAX 16

class CUser
{
public:

private:
	wchar_t m_name[NAME_MAX];
public:
	CUser();
	~CUser();
	CUser(CUser* _recvBuffer);

	wchar_t* GetName() { return m_name; }
};