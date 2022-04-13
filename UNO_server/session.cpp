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
	if (m_pRoom) { delete m_pRoom; m_pRoom = nullptr; }
	if (m_pUser) { delete m_pUser; m_pUser = nullptr; }
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
	case CS_PT_CREATEROOM: // 수정 : 성공 실패에 대한 send
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


void CSession::NickName() // 처음으로 recv
{
	CUserManager* pUserManager = CUserManager::GetInstance();

	memcpy(m_pUser, m_buffer + 4, sizeof(CUser));
	wprintf(L"%s \n", m_pUser->GetName());

	pUserManager->GetUserList()->push_back(m_pUser);

	CUserManager::userList_t userList = *pUserManager->GetUserList();

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
	CRoomManager* pRoomManager = CRoomManager::GetInstance();

	CRoomManager::roomList_t roomList = *CRoomManager::GetInstance()->GetRoomList();
	
	int roomNum = roomList.size() + 1;

	wchar_t tempBuffer[1000];

	memcpy(tempBuffer, m_buffer, 1000);

	m_pRoom = new CRoom(roomNum, tempBuffer + 4);

	// 여기부터 시작. 2022-04-12

	pRoomManager->GetRoomList()->push_back(new CRoom(roomNum, tempBuffer));

	//send 
}


void CSession::UserList()
{
	CUserManager::userList_t userList = *CUserManager::GetInstance()->GetUserList();
	char sendBuffer[1000];
	char* tempBuffer = sendBuffer;

	int listSize = userList.size();

	*(unsigned short*)tempBuffer = 2 + 2 + (sizeof(CUser) * listSize);
	tempBuffer += sizeof(unsigned short);
	*(unsigned short*)tempBuffer = CS_PT_USERLIST;
	tempBuffer += sizeof(unsigned short);

	std::list<CUser*>::iterator iter = userList.begin();
	int len = sizeof(CUser);
	for (; iter != userList.end(); iter++) // 첫 화면에는 15개만
	{
		memcpy(tempBuffer, *iter, len);
		tempBuffer += len;
	}

	int bufferSize = tempBuffer - sendBuffer + (len * listSize);

	send(m_socket, sendBuffer, bufferSize, 0);
}

void CSession::RoomList()
{
	CRoomManager::roomList_t roomList = *CRoomManager::GetInstance()->GetRoomList();
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
