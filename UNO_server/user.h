#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>

class CUser //Ŀ�ؼ�, ����
{
public:                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
private:
	SOCKET m_socket;
	SOCKADDR_IN m_addr;

public:
	CUser();
	~CUser();
	CUser(SOCKET _socket, SOCKADDR_IN& _addr);

	SOCKET GetSock() { return m_socket; }
private:
};