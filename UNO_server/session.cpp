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
	m_list = CInformation::GetInstance()->GetList();
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
		m_list->userlist.push_back(new CUser(m_buffer));
	}
	if (_type == CREATE_ROOM)
	{

	}
	if (_type == USERLIST)
	{
		char buffer[255];
		int count = 0;
		std::list<CUser*>::iterator iter = m_list->userlist.begin();
		int bufferlen = 0;
		for (; iter != m_list->userlist.end(); iter++)
		{
			CUser* temp = iter.operator*();

			int len = strlen(temp->GetName()) + 1;
			//strcat_s(recvBuffer, temp->GetName());

			memcpy(buffer + bufferlen, temp->GetName(), len);
			bufferlen += len;
			count++;
		}

		char sendBuffer[255]; // Å©±â´Â?
		char* tempBuffer = sendBuffer;

		*(unsigned short*)tempBuffer = 2 + 2 + bufferlen;
		tempBuffer += sizeof(unsigned short);
		*(unsigned short*)tempBuffer = _type;
		tempBuffer += sizeof(unsigned short);
		//*tempBuffer = count;
		//tempBuffer += sizeof(unsigned char);

		memcpy(tempBuffer, buffer, bufferlen);
		int bufferSize = tempBuffer - sendBuffer + bufferlen;

		send(m_socket, sendBuffer, bufferSize, 0);
	}
}
