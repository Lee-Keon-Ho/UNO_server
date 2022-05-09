#include "session.h"
#include "userManager.h"
#include "roomManager.h"
#include "PacketType.h"
#include <stdio.h>

#define SENDBUFFER 1000
#define CHATBUFFER 64
CSession::CSession()
{
	m_pUser = new CUser();
}

CSession::~CSession()
{
	if (m_pUser) { delete m_pUser; m_pUser = nullptr; }
	closesocket(m_socket);
}

CSession::CSession(SOCKET _socket, SOCKADDR_IN& _addr)
	: m_socket(_socket), m_addr(_addr)
{
	m_pUser = new CUser();
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

			if (recvedSize > 1) 
			{
				packetSize = *(unsigned short*)m_buffer;
			}
			if (recvedSize > 3)
			{
				type = *(unsigned short*)(m_buffer + 2);
			}
		}
	}

	return recvLen;
}

void CSession::HandlePacket(int _type)
{
	switch (_type)
	{
	case CS_PT_LOGIN:
		Login();
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
	case CS_PT_INROOM:
		RoomIn();
		break;
	case CS_PT_OUTROOM:
		RoomOut(m_socket);
		break;
	case CS_PT_ROOMSTATE:
		RoomState();
		break;
	case CS_PT_CHATTING:
		Chatting();
		break;
	}
}

void CSession::Login()
{
	CUserManager* pUserManager = CUserManager::GetInstance();

	m_pUser->SetName(m_buffer + 4);

	pUserManager->GetUserList()->push_back(m_pUser);

	CUserManager::userList_t userList = *pUserManager->GetUserList();

	char sendBuffer[SENDBUFFER];
	char* tempBuffer = sendBuffer;

	int listSize = userList.size();

	int len = sizeof(wchar_t) * NAME_MAX;

	*(unsigned short*)tempBuffer = 2 + 2 + (len * listSize);
	tempBuffer += sizeof(unsigned short);
	*(unsigned short*)tempBuffer = CS_PT_LOGIN;
	tempBuffer += sizeof(unsigned short);

	std::list<CUser*>::iterator iter = userList.begin();
	for (; iter != userList.end(); iter++)
	{
		memcpy(tempBuffer, (*iter)->GetName(), len);
		tempBuffer += len;
	}

	int bufferSize = tempBuffer - sendBuffer;

	send(m_socket, sendBuffer, bufferSize, 0);
}

void CSession::CreateRoom()
{
	char* tempBuffer = m_buffer;
	char sendBuffer[SENDBUFFER];
	char* sendTempBuffer = sendBuffer;
	bool bCreate = true;
	tempBuffer += (sizeof(unsigned short) * 2); // 4;
	
	m_pUser->SetImage(*tempBuffer);
	tempBuffer += sizeof(unsigned short);

	if (m_pUser->CreateRoom(tempBuffer))
	{
		m_pUser->PlayerIn(m_socket);
		bCreate = true;
	}
	else 
	{ 
		bCreate = false;
	}

	*(unsigned short*)sendTempBuffer = 2 + 2 + 2;
	sendTempBuffer += sizeof(unsigned short);
	*(unsigned short*)sendTempBuffer = CS_PT_CREATEROOM;
	sendTempBuffer += sizeof(unsigned short);
	*(unsigned short*)sendTempBuffer = bCreate;
	sendTempBuffer += sizeof(unsigned short);

	int bufferSize = sendTempBuffer - sendBuffer;

	send(m_socket, sendBuffer, bufferSize, 0);
}

void CSession::UserList()
{
	CUserManager::userList_t userList = *CUserManager::GetInstance()->GetUserList();
	char sendBuffer[1000];
	char* tempBuffer = sendBuffer;

	int listSize = userList.size();

	int len = sizeof(wchar_t) * NAME_MAX;

	*(unsigned short*)tempBuffer = 2 + 2 + (len * listSize);
	tempBuffer += sizeof(unsigned short);
	*(unsigned short*)tempBuffer = CS_PT_USERLIST;
	tempBuffer += sizeof(unsigned short);

	std::list<CUser*>::iterator iter = userList.begin();
	for (; iter != userList.end(); iter++) // 첫 화면에는 15개만
	{
		memcpy(tempBuffer, (*iter)->GetName(), len);
		tempBuffer += len;
	}

	int bufferSize = tempBuffer - sendBuffer;

	send(m_socket, sendBuffer, bufferSize, 0);
}


void CSession::RoomList()
{
	CRoomManager* pRM = CRoomManager::GetInstance();
	CRoomManager::roomList_t* roomList = pRM->GetRoomList();
	char sendBuffer[SENDBUFFER];
	char* tempBuffer = sendBuffer;

	int listSize = pRM->GetCount();
	int len = sizeof(CRoom::stROOM);

	*(unsigned short*)tempBuffer = 2 + 2 + (len * listSize);
	tempBuffer += sizeof(unsigned short);
	*(unsigned short*)tempBuffer = CS_PT_ROOMLIST;
	tempBuffer += sizeof(unsigned short);

	std::vector<CRoom*>::iterator iter = roomList->begin();
	for(int i = 0; i < listSize; iter++, i++) // 2022-04-25 수정 : 보여지는 화면만 8개(i 값은 추후에 변경)
	{
		memcpy(tempBuffer, (*iter)->GetInfo(), len);
		tempBuffer += len;
	}

	int bufferSize = tempBuffer - sendBuffer;

	send(m_socket, sendBuffer, bufferSize, 0);
}

void CSession::RoomIn()
{
	char* tempBuffer = m_buffer;
	char sendBuffer[SENDBUFFER];
	char* sendTempBuffer = sendBuffer;
	bool bRoomIn = true;

	tempBuffer += (sizeof(unsigned short) * 2); // 4;

	if (m_pUser->RoomIn(tempBuffer, m_socket)) bRoomIn = true;
	else bRoomIn = false;
	
	*(unsigned short*)sendTempBuffer = 2 + 2 + 2;
	sendTempBuffer += sizeof(unsigned short);
	*(unsigned short*)sendTempBuffer = CS_PT_INROOM;
	sendTempBuffer += sizeof(unsigned short);
	*(unsigned short*)sendTempBuffer = bRoomIn;
	sendTempBuffer += sizeof(unsigned short);

	int bufferSize = sendTempBuffer - sendBuffer;

	send(m_socket, sendBuffer, bufferSize, 0);
}

void CSession::RoomOut(SOCKET _socket)
{
	m_pUser->RoomOut(_socket);
}

void CSession::RoomState()
{
	char sendBuffer[SENDBUFFER];
	char* tempBuffer = sendBuffer;

	int playerCount = m_pUser->GetPlayerCount();
	int size = sizeof(CRoom::stUSER) * PLAYER_MAX;
	*(unsigned short*)tempBuffer = 2 + 2 + sizeof(unsigned short) + size;
	tempBuffer += sizeof(unsigned short);
	*(unsigned short*)tempBuffer = CS_PT_ROOMSTATE;
	tempBuffer += sizeof(unsigned short);

	*(unsigned short*)tempBuffer = playerCount;
	tempBuffer += sizeof(unsigned short);

	memcpy(tempBuffer, m_pUser->GetInRoomUserInfo(), size);

	int bufferSize = tempBuffer - sendBuffer + size;

	send(m_socket, sendBuffer, bufferSize, 0);
}

void CSession::Chatting()
{
	// 2022-04-29 수정
	char buffer[CHATBUFFER];
	memset(buffer, 0, CHATBUFFER);
	char* tempBuffer = buffer;

	unsigned short packetSize = *(unsigned short*)m_buffer;

	int size = wcslen(m_pUser->GetName()) * sizeof(wchar_t);
	memcpy(tempBuffer, m_pUser->GetName(), size);
	tempBuffer += size;
	memcpy(tempBuffer, L" : ", wcslen(L" : ") * sizeof(wchar_t));
	tempBuffer += wcslen(L" : ") * sizeof(wchar_t);
	memcpy(tempBuffer, m_buffer + 4, packetSize - 4);
	tempBuffer += packetSize - 4;

	char sendBuffer[BUFFER_MAX];
	char* sendTempBuffer = sendBuffer;

	int sendLen = 2 + 2 + tempBuffer - buffer;

	*(unsigned short*)sendTempBuffer = sendLen; // 2022-04-29 test
	sendTempBuffer += sizeof(unsigned short);
	*(unsigned short*)sendTempBuffer = CS_PT_CHATTING;
	sendTempBuffer += sizeof(unsigned short);
	memcpy(sendTempBuffer, buffer, tempBuffer - buffer);

	for (int i = 0; i < m_pUser->GetPlayerCount(); i++)
	{
		int sendSize = send(m_pUser->GetInRoomUserInfo()[i].socket, sendBuffer, sendLen, 0);
		if (sendSize < 0)
		{
			break;
		}
	}
}