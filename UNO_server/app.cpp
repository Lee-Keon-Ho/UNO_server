#include "app.h"
#pragma comment( lib, "ws2_32.lib")

CApp::CApp()
{
}

CApp::~CApp()
{
}

bool CApp::Initialize(const char* _ip, int _port)
{
	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("Failed WSAStartup() \n");
		return false;
	}

	m_sockServer = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_sockServer == INVALID_SOCKET)
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
	if (bind(m_sockServer, (SOCKADDR*)&addrServer, sizeof(addrServer)) == SOCKET_ERROR)
	{
		printf("Binding Error \n");
		closesocket(m_sockServer);
		WSACleanup();
		return false;
	}
	if (listen(m_sockServer, 5) == SOCKET_ERROR)
	{
		printf("listen Error \n");
		closesocket(m_sockServer);
		WSACleanup();
		return false;
	}

	printf("Start Server \n");
	return true;
}

void CApp::Cleanup()
{
	closesocket(m_sockServer);
	WSACleanup();
}

void CApp::Update()
{

}
