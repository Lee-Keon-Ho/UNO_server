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

void CServer::Cleanup()
{
	closesocket(m_sockServer);
	WSACleanup();
}

void CServer::Update()
{
	fd_set fdReads;
	int iRet;
	unsigned int i;
	SOCKET sockTemp;
	int recvSize;

	FD_ZERO_EX(&m_fdSocketInfors);
	FD_SET_EX(m_sockServer, nullptr, &m_fdSocketInfors);

	SOCKET  socketClient;
	while (1)
	{
		fdReads = m_fdSocketInfors;
		iRet = select(0, &fdReads, 0, 0, 0);
		if (iRet == SOCKET_ERROR) return;
		for (i = 0; i < m_fdSocketInfors.fd_count; i++)
		{
			sockTemp = m_fdSocketInfors.fd_array[i];
			if (FD_ISSET(sockTemp, &fdReads))
			{
				if (sockTemp == m_sockServer)
				{
					Accept();
				}
				else
				{
					recvSize = Recv(i);
					if (recvSize < 0)
					{
						delete m_fdSocketInfors.user_array[i];
						m_fdSocketInfors.user_array[i] = nullptr;
						FD_CLR_EX(sockTemp, &m_fdSocketInfors);
					}
				}
			}
		}
	}
}

void CServer::Accept()
{
	SOCKET sockClient;
	SOCKADDR_IN addrClient;
	int addrSize;
	int count = m_fdSocketInfors.fd_count;
	addrSize = sizeof(addrClient);
	sockClient = accept(m_sockServer, (SOCKADDR*)&addrClient, &addrSize);
	m_fdSocketInfors.user_array[count] = new CUser(sockClient, addrClient);
	FD_SET(sockClient, &m_fdSocketInfors);
}

int CServer::Recv(int _index)
{
	return m_fdSocketInfors.user_array[_index]->Recv();
}
