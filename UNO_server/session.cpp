#include "session.h"
#include "roomManager.h"
#include "userManager.h"
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
			packetSize = *(unsigned short*)m_buffer;
			type = *(unsigned short*)(m_buffer + 2);
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
	case CS_PT_DESTROYROOM:
		DestroyRoom();
		break;
	}
}


void CSession::Login()
{
	CUserManager* pUserManager = CUserManager::GetInstance();

	m_pUser->SetName(m_buffer + 4);

	wprintf(L"%s", m_pUser->GetName());
	printf(" 서버 접속\n");
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
	CRoomManager* pRoomManager = CRoomManager::GetInstance();

	CRoomManager::roomList_t roomList = *pRoomManager->GetRoomList();
	
	int roomNum = roomList.size();
	CRoomManager::roomList_t::iterator iter = roomList.begin();

	// 2022-04-19 수정 : test 좀 더 좋은 방법을 생각해보자
	if (roomNum == 0)
	{
		roomNum = 1;
	}
	else if (roomNum == 1)
	{
		if ((*iter)->GetInfo()->number != 1)
		{
			roomNum = 1;
		}
		else
		{
			roomNum = 2;
		}
	}
	else
	{
		for (int i = 2; iter != roomList.end(); iter++, i++)
		{
			if ((*iter)->GetInfo()->number != i)
			{
				roomNum = i;
			}
		}
	}

	m_pUser->SetRoomInfo(m_buffer + 4, roomNum);

	wprintf(L"%s", m_pUser->GetName());
	printf(" 방 생성\n");

	pRoomManager->GetRoomList()->push_back(m_pUser->GetRoom());
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
	CRoomManager::roomList_t roomList = *CRoomManager::GetInstance()->GetRoomList();
	char sendBuffer[1000];
	char* tempBuffer = sendBuffer;

	int listSize = roomList.size();

	*(unsigned short*)tempBuffer = 2 + 2 + (sizeof(CRoom::stROOM) * listSize);
	tempBuffer += sizeof(unsigned short);
	*(unsigned short*)tempBuffer = CS_PT_ROOMLIST;
	tempBuffer += sizeof(unsigned short);

	std::list<CRoom*>::iterator iter = roomList.begin();
	int len = sizeof(CRoom::stROOM);
	for(; iter != roomList.end(); iter++) // 수정 : 보여지는 화면만 8개
	{
		memcpy(tempBuffer, (*iter)->GetInfo(), len);
		tempBuffer += len;
	}

	int bufferSize = tempBuffer - sendBuffer;

	wprintf(L"%s ", m_pUser->GetName());
	printf("RoomList 갱신\n");

	send(m_socket, sendBuffer, bufferSize, 0);
}

void CSession::DestroyRoom()
{
	CRoomManager* pRoomManager = CRoomManager::GetInstance();

	CRoomManager::roomList_t* roomList = pRoomManager->GetRoomList();

	wprintf(L"%s", m_pUser->GetName());
	printf(" 방 파괴\n");

	m_pUser->DestroyRoom(roomList);
	
	//send 수정
}