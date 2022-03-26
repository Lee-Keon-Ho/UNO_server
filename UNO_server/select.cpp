#include "select.h"
#include <stdio.h>
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
						delete m_fdSocketInfors.session_array[i];
						m_fdSocketInfors.session_array[i] = nullptr;
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
	m_fdSocketInfors.session_array[count] = new CSession(sockClient, addrClient);
	FD_SET(sockClient, &m_fdSocketInfors);

	//test
	char sendBuffer[] = { "hi" };
	for (int i = 1; i < m_fdSocketInfors.fd_count; i++)
	{
		int sendLen = send(m_fdSocketInfors.fd_array[i], sendBuffer, strlen(sendBuffer) + 1, 0);
		if (sendLen < 0); // ����
	}
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
		//���⼭ ó�� type���� ó��
		// �̰� �г����̾� -> ó�� ���� -> roomlist, userlist, chatting
		//
		// roomlist
		// �� room�� ���� �� room�� ������ ������ ���� ������Ȳ
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
	if (_type == NICK_NAME)
	{
		userlist.push_back(new CUser(_recvBuffer));

		char buffer[255];
		int count = 0;
		UserList::iterator iter = userlist.begin();
		int bufferlen = 0;
		for (; iter != userlist.end(); iter++)
		{
			CUser* temp = iter.operator*();
			
			int len = strlen(temp->GetName()) + 1;
			//strcat_s(recvBuffer, temp->GetName());

			memcpy(buffer + bufferlen, temp->GetName(), len);
			bufferlen += len;
		}

		char sendBuffer[255]; // ũ���?
		char* tempBuffer = sendBuffer;

		*(unsigned short*)tempBuffer = 2 + 2 + bufferlen;
		tempBuffer += sizeof(unsigned short);
		*(unsigned short*)tempBuffer = _type;
		tempBuffer += sizeof(unsigned short);

		memcpy(tempBuffer, buffer, bufferlen);
		int bufferSize = tempBuffer - sendBuffer + bufferlen;

		// sendAll
		for (int i = 0; i < m_fdSocketInfors.fd_count; i++)
		{
			int sendLen = send(m_fdSocketInfors.fd_array[i], sendBuffer, bufferSize, 0);
		}
	}
	if (_type == CREATE_ROOM)
	{
		//roomlist.
		roomlist.push_back(new CRoom(_recvBuffer));
		// userlist.setUser(_recvBuffer);
		// class roomlist, userlist �� class���� send�� ���ָ� �ȴ�.
		// send(m_socket)
	}
}

void CSelect::Remove(int _num)
{

}