#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>

class CUser
{
public:
	enum TYPE
	{
		NICK_NAME = 1,
		CHATTING
	};                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
private:
	SOCKET m_clientSock;
	SOCKADDR_IN m_addrClient;
	char m_name[5];

public:
	CUser();
	~CUser();
private:
};