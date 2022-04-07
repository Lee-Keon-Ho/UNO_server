#pragma once
#include "select.h"
#include <WinSock2.h>
#include <WS2tcpip.h>

class CServer
{
public:
private:
	SOCKET m_listenSocket;
	CSelect* m_select;

public:
	CServer();
	~CServer();

	bool Initialize(const char* _ip, int _port);
	void Cleanup();

	void Update();
private:
};