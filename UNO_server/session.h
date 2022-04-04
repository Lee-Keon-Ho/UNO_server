#pragma once
#include "Information.h"
#include <WinSock2.h>
#include <WS2tcpip.h>

#define BUFFER_MAX 1000

class CSession //目池记, 技记
{
public:
	enum TYPE
	{
		NICK_NAME = 1,
		CREATE_ROOM,
		USERLIST,
		MAX
	};

private:
	SOCKET m_socket;
	SOCKADDR_IN m_addr;
	char m_buffer[BUFFER_MAX];

	CInformation::LIST* m_list;
public:
	CSession();
	~CSession();
	CSession(SOCKET _socket, SOCKADDR_IN& _addr);

	SOCKET GetSock() { return m_socket; }
	int Recv();
	void HandlePacket(int _type);
private:
};