#include "user.h"
#include <stdio.h>

#define BUFFER_MAX 255

CUser::CUser() : m_recvedSize(0)
{
}

CUser::~CUser()
{
}

CUser::CUser(SOCKET _socket, SOCKADDR_IN& _addr)
{
	m_clientSock = _socket;
	m_addrClient = _addr;
	m_pRecvBuffer = new char[BUFFER_MAX];
	memset(m_pRecvBuffer, 0, BUFFER_MAX);
	m_recvedSize = 0;
}


int CUser::Recv()
{
	int recvLen = recv(m_clientSock, m_pRecvBuffer + m_recvedSize, BUFFER_MAX - m_recvedSize, 0);
	if (recvLen < 1)  return recvLen;

	m_recvedSize += recvLen;

	if (m_recvedSize < 2)  return recvLen;

	unsigned short packetSize = *(unsigned short*)m_pRecvBuffer;
	unsigned short type = *(unsigned short*)(m_pRecvBuffer + 2);

	while (m_recvedSize >= packetSize)
	{
		//HandlePacket(m_recvBuffer); 
		//여기서 처리를 하는 거지 무조건 적으로 데이터를 보낼 필요가 없다

		m_recvedSize -= packetSize;
		if (m_recvedSize > 0)
		{
			memcpy(m_pRecvBuffer, m_pRecvBuffer + packetSize, m_recvedSize);
			m_recvedSize = m_pRecvBuffer[0];
		}
	}

	return recvLen;
}