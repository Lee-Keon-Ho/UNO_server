#pragma once

class CUser
{
public:
private:
	// � clientSocket���� �˾ƾ� �ϳ�?
	char* m_pName;
public:
	CUser();
	~CUser();
	CUser(char* _recvBuffer);
};