#pragma once
#pragma once
#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

#define MAX 64

class CApp
{
public:
private:
	SOCKET m_sockServer;
	
public:
	CApp();
	~CApp();

	bool Initialize(const char* _ip, int _port);
	void Cleanup();

	void Update();

private:
};