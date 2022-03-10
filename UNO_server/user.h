#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>

class CUser
{
public:                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
private:
	SOCKET m_clientSock;
	SOCKADDR_IN m_addrClient;
	char* m_pRecvBuffer;
	int m_recvedSize;

public:
	CUser();
	~CUser();
	CUser(SOCKET _socket, SOCKADDR_IN& _addr);

	int Recv();
private:
};