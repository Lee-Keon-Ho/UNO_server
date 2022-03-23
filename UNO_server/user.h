#pragma once

class CUser
{
public:
private:
	//
	char* m_pName;
public:
	CUser();
	~CUser();
	CUser(char* _recvBuffer);
};