#include "session.h"
#include <stdio.h>

CSession::CSession()
{
}

CSession::~CSession()
{
}

CSession::CSession(SOCKET _socket, SOCKADDR_IN& _addr)
	: m_socket(_socket), m_addr(_addr)
{
	m_pList = CInformation::GetInstance()->GetList();
}

int CSession::Recv()
{
	int recvedSize = 0;
	int recvLen = recv(m_socket, m_buffer + recvedSize, BUFFER_MAX - recvedSize, 0);
	if (recvLen < 1)  return recvLen;

	recvedSize += recvLen;

	if (recvedSize < 2)  return recvLen;

	unsigned short packetSize = *(unsigned short*)m_buffer;
	unsigned short type = *(unsigned short*)(m_buffer + 2);

	while (recvedSize >= packetSize)
	{
		HandlePacket(type);

		recvedSize -= packetSize;
		if (recvedSize > 0)
		{
			memcpy(m_buffer, m_buffer + packetSize, recvedSize);
			recvedSize = m_buffer[0];
		}
	}

	return recvLen;
}

void CSession::HandlePacket(int _type)
{
	if (_type == NICK_NAME)
	{
		m_pList->userlist.push_back(new CUser(m_buffer));

		/*式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式*/
		char sendBuffer[1000];
		char* tempBuffer = sendBuffer;

		int listSize = m_pList->userlist.size();

		*(unsigned short*)tempBuffer = 2 + 2 + (sizeof(CUser) * listSize);
		tempBuffer += sizeof(unsigned short);
		*(unsigned short*)tempBuffer = _type;
		tempBuffer += sizeof(unsigned short);

		//std::list<CUser*>::iterator 
		std::list<CUser*>::iterator iter = m_pList->userlist.begin();
		int len = sizeof(CUser);
		for (; iter != m_pList->userlist.end(); iter++)
		{
			CUser* temp = iter.operator*();
		
			memcpy(tempBuffer, temp, len);
			tempBuffer += len;
		}

		int bufferSize = tempBuffer - sendBuffer + (len * listSize);

		send(m_socket, sendBuffer, bufferSize, 0);
	}
	if (_type == CREATE_ROOM)
	{

	}
	if (_type == USERLIST)
	{
		char sendBuffer[1000];
		char* tempBuffer = sendBuffer;

		int listSize = m_pList->userlist.size();

		*(unsigned short*)tempBuffer = 2 + 2 + (sizeof(CUser) * listSize);
		tempBuffer += sizeof(unsigned short);
		*(unsigned short*)tempBuffer = _type;
		tempBuffer += sizeof(unsigned short);

		//std::list<CUser*>::iterator 
		std::list<CUser*>::iterator iter = m_pList->userlist.begin();
		int len = sizeof(CUser);
		for (; iter != m_pList->userlist.end(); iter++)
		{
			CUser* temp = iter.operator*();

			memcpy(tempBuffer, temp, len);
			tempBuffer += len;
		}

		int bufferSize = tempBuffer - sendBuffer + (len * listSize);

		send(m_socket, sendBuffer, bufferSize, 0);
	}
	if (_type == ROOMLIST)
	{
		
	}
}
