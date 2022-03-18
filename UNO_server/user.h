#pragma once

class CUser
{
public:
private:
	// 어떤 clientSocket인지 알아야 하나?
	char* m_pName;
public:
	CUser();
	~CUser();
	CUser(char* _recvBuffer);
};