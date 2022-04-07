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
	switch(_type)
	{
	case CS_PT_NICKNAME:
		NickName();
		break;
	case CS_PT_CREATEROOM:
		CreateRoom();
		break;
	case CS_PT_USERLIST:
		UserList();
		break;
	case CS_PT_ROOMLIST:
		RoomList();
		break;
	}
}


void CSession::NickName()
{
	CInformation* pInformation = CInformation::GetInstance();
	pInformation->GetUserList()->push_back(new CUser(m_buffer));

	CInformation::UserList userList = *pInformation->GetUserList();
	
	char sendBuffer[1000];
	char* tempBuffer = sendBuffer;

	int listSize = userList.size();

	*(unsigned short*)tempBuffer = 2 + 2 + (sizeof(CUser) * listSize);
	tempBuffer += sizeof(unsigned short);
	*(unsigned short*)tempBuffer = CS_PT_NICKNAME;
	tempBuffer += sizeof(unsigned short);

	std::list<CUser*>::iterator iter = userList.begin();
	int len = sizeof(CUser);
	for (; iter != userList.end(); iter++)
	{
		memcpy(tempBuffer, *iter, len);
		tempBuffer += len;
	}

	int bufferSize = tempBuffer - sendBuffer + (len * listSize);

	send(m_socket, sendBuffer, bufferSize, 0);
}


void CSession::CreateRoom()
{
	CInformation* pInformation = CInformation::GetInstance();
	
	CInformation::RoomList roomList = *pInformation->GetRoomList();
	
	int roomNum = roomList.size() + 1;

	pInformation->GetRoomList()->push_back(new CRoom(roomNum, m_buffer));
}


void CSession::UserList()
{
	CInformation::UserList userList = *CInformation::GetInstance()->GetUserList();
	char sendBuffer[1000];
	char* tempBuffer = sendBuffer;

	int listSize = userList.size();

	*(unsigned short*)tempBuffer = 2 + 2 + (sizeof(CUser) * listSize);
	tempBuffer += sizeof(unsigned short);
	*(unsigned short*)tempBuffer = CS_PT_USERLIST;
	tempBuffer += sizeof(unsigned short);

	std::list<CUser*>::iterator iter = userList.begin();
	int len = sizeof(CUser);
	for (; iter != userList.end(); iter++) // 정확히 몇개가의 기준이 필요하다
	{
		memcpy(tempBuffer, *iter, len);
		tempBuffer += len;
	}

	int bufferSize = tempBuffer - sendBuffer + (len * listSize);

	send(m_socket, sendBuffer, bufferSize, 0);
}

void CSession::RoomList()
{
	CInformation::RoomList roomList = *CInformation::GetInstance()->GetRoomList();
	char sendBuffer[1000];
	char* tempBuffer = sendBuffer;

	int listSize = roomList.size();

	*(unsigned short*)tempBuffer = 2 + 2 + (sizeof(CRoom) * listSize);
	tempBuffer += sizeof(unsigned short);
	*(unsigned short*)tempBuffer = CS_PT_ROOMLIST;
	tempBuffer += sizeof(unsigned short);

	std::list<CRoom*>::iterator iter = roomList.begin();
	int len = sizeof(CRoom);
	for(; iter != roomList.end(); iter++)
	{
		memcpy(tempBuffer, *iter, len);
		tempBuffer += len;
	}

	int bufferSize = tempBuffer - sendBuffer + (len * listSize);

	send(m_socket, sendBuffer, bufferSize, 0);
}
