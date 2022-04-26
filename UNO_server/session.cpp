#include "session.h"
#include "userManager.h"
#include "roomManager.h"
#include "PacketType.h"
#include <stdio.h>

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

			// 2022-04-25 수정 : unsigned short 값이면 packetSize
			if (recvedSize > 1) 
			{
				packetSize = *(unsigned short*)m_buffer;
			}
			// 2022-04-25 수정 : recvedSize가 3초과면 type까지 알 수 있다.
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
	switch(_type)
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
		InRoom();
		break;
	case CS_PT_OUTROOM:
		OutRoom();
		break;
	case CS_PT_ROOMSTATE:
		RoomState();	
		break;
	}
}


void CSession::Login()
{
	CUserManager* pUserManager = CUserManager::GetInstance();

	m_pUser->SetName(m_buffer + 4);

	wprintf(L"%s", m_pUser->GetName());
	printf(" 접속\n");
	pUserManager->GetUserList()->push_back(m_pUser);

	CUserManager::userList_t userList = *pUserManager->GetUserList();

	char sendBuffer[1000];
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
	
	tempBuffer += (sizeof(unsigned short) * 2); // 4;
	
	m_pUser->SetImage(*tempBuffer);
	tempBuffer += sizeof(unsigned short);

	m_pUser->SetRoom(CRoomManager::GetInstance()->CreateRoom(tempBuffer));
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

	wprintf(L"%s ", m_pUser->GetName());
	printf("UserList 갱신\n");

	send(m_socket, sendBuffer, bufferSize, 0);
}


void CSession::RoomList()
{
	CRoomManager* pRM = CRoomManager::GetInstance();
	CRoomManager::roomList_t* roomList = pRM->GetRoomList();
	char sendBuffer[1000];
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

void CSession::InRoom()
{
	char* tempBuffer = m_buffer;

	tempBuffer += (sizeof(unsigned short) * 2); // 4;

	m_pUser->SetRoom(CRoomManager::GetInstance()->InRoom(tempBuffer));
}

void CSession::OutRoom()
{
	CRoomManager::GetInstance()->OutRoom(m_pUser->GetRoomNumber());
	m_pUser->SetRoom(nullptr);
}

void CSession::RoomState()
{
	//CRoomBuffer::GetInstance()->RoomState();
}
/*
void CSession::RoomState()
{
	char sendBuffer[1000];
	char* tempBuffer = sendBuffer;

	int len = sizeof(CRoom::stROOM);

	*(unsigned short*)tempBuffer = 2 + 2 + len;
	tempBuffer += sizeof(unsigned short);
	*(unsigned short*)tempBuffer = CS_PT_ROOMSTATE;
	tempBuffer += sizeof(unsigned short);

	memcpy(tempBuffer, m_pUser->GetRoom()->GetInfo(), len);

	int bufferSize = tempBuffer - sendBuffer + len;

	send(m_socket, sendBuffer, bufferSize, 0);
}*/