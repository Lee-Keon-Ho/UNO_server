#include "select.h"
#pragma comment( lib, "ws2_32.lib")
#define BUFFER_MAX 255

CSelect::CSelect()
{

}

CSelect::~CSelect()
{

}

CSelect::CSelect(SOCKET _listenSocket) 
	: m_listenSocket(_listenSocket)
{
	
}

void CSelect::Update()
{
	fd_set fdReads;
	int iRet;
	unsigned int i;
	SOCKET sockTemp;
	int recvSize;

	FD_ZERO_EX(&m_fdSocketInfors);
	FD_SET_EX(m_listenSocket, nullptr, &m_fdSocketInfors);

	while (1)
	{
		fdReads = (fd_set)m_fdSocketInfors;
		iRet = select(0, &fdReads, 0, 0, 0);
		if (iRet == SOCKET_ERROR) return;
		for (i = 0; i < m_fdSocketInfors.fd_count; i++)
		{
			sockTemp = m_fdSocketInfors.fd_array[i];
			if (FD_ISSET(sockTemp, &fdReads))
			{
				if (sockTemp == m_listenSocket)
				{
					Accept();
				}
				else
				{
					recvSize = Recv(sockTemp);
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

void CSelect::Accept()
{
	SOCKET sockClient;
	SOCKADDR_IN addrClient;
	int addrSize;
	int count = m_fdSocketInfors.fd_count;
	addrSize = sizeof(addrClient);
	sockClient = accept(m_listenSocket, (SOCKADDR*)&addrClient, &addrSize);
	m_fdSocketInfors.user_array[count] = new CUser(sockClient, addrClient);
	FD_SET(sockClient, &m_fdSocketInfors);
}

int CSelect::Recv(SOCKET _socket)
{
	char recvBuffer[BUFFER_MAX];
	int recvedSize = 0;
	int recvLen = recv(_socket, recvBuffer + recvedSize, BUFFER_MAX - recvedSize, 0);
	if (recvLen < 1)  return recvLen;

	recvedSize += recvLen;

	if (recvedSize < 2)  return recvLen;

	unsigned short packetSize = *(unsigned short*)recvBuffer;
	unsigned short type = *(unsigned short*)(recvBuffer + 2);

	while (recvedSize >= packetSize)
	{
		HandlePacket(recvBuffer, type);
		//HandlePacket(m_recvBuffer);
		//여기서 처리 type별로 처리
		// 이건 닉네임이야 -> 처음 접속 -> roomlist, userlist, chatting
		//
		// roomlist
		// 총 room의 개수 각 room의 접속한 유저수 게임 진행현황
		//
		recvedSize -= packetSize;
		if (recvedSize > 0)
		{
			memcpy(recvBuffer, recvBuffer + packetSize, recvedSize);
			recvedSize = recvBuffer[0];
		}
	}

	return recvLen;
}

void CSelect::HandlePacket(char* _recvBuffer, int _type)
{
	if (_type) // 접속이면
	{
		// userlist.setUser(_recvBuffer);
		// class roomlist, userlist 각 class에서 send를 해주면 된다.
		// send(m_socket)
	}
}