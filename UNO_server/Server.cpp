#include "Server.h"
#include <stdio.h>
#pragma comment( lib, "ws2_32.lib")

CServer::CServer()
{
}

CServer::~CServer()
{
}

bool CServer::Initialize(const char* _ip, int _port)
{
	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("Failed WSAStartup() \n");
		return false;
	}

	m_listenSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_listenSocket == INVALID_SOCKET)
	{
		WSACleanup();
		printf("Failed socket() \n");
		return false;
	}

	SOCKADDR_IN addrServer;
	memset(&addrServer, 0, sizeof(addrServer));
	addrServer.sin_family = AF_INET;
	inet_pton(AF_INET, _ip, &addrServer.sin_addr);
	addrServer.sin_port = htons(_port);
	if (bind(m_listenSocket, (SOCKADDR*)&addrServer, sizeof(addrServer)) == SOCKET_ERROR)
	{
		printf("Binding Error \n");
		closesocket(m_listenSocket);
		WSACleanup();
		return false;
	}
	if (listen(m_listenSocket, 5) == SOCKET_ERROR)
	{
		printf("listen Error \n");
		closesocket(m_listenSocket);
		WSACleanup();
		return false;
	}

	m_select = new CSelect(m_listenSocket);

	printf("Start Server \n");
	return true;
}

void CServer::Cleanup()
{
	closesocket(m_listenSocket);
	WSACleanup();
}

void CServer::Update()
{
	m_select->Update();
}