#include "User.h"
#include "PacketType.h"
#include "userManager.h"
#include "roomManager.h"
#include <stdio.h>
#include <string.h>

#define SENDBUFFER 2000

CUser::CUser()
{
	
}

CUser::CUser(SOCKET _socket, SOCKADDR_IN& _addr)
	: CSession(_socket, _addr)
{
}

CUser::~CUser()
{
	CSession::~CSession();
}

void CUser::PlayerIn(SOCKET _socket)
{ 
	m_pRoom->PlayerIn(m_name, m_image, _socket);
}

void CUser::SetName(char* _name)
{
	memcpy(m_name, _name, sizeof(wchar_t) * NAME_MAX);
}

bool CUser::CreateRoom(char* _name)
{
	m_pRoom = CRoomManager::GetInstance()->CreateRoom(_name);
	if (m_pRoom == nullptr)
	{
		return false;
	}
	return true;
}

bool CUser::RoomIn(char* _playerInfo, SOCKET _socket)
{
	m_pRoom = CRoomManager::GetInstance()->RoomIn(_playerInfo, _socket);
	if (m_pRoom == nullptr)
	{
		return false;
	}
	return true;
}

void CUser::RoomOut(SOCKET _socket)
{
	// 2022-05-17 수정하자.... 2022-05-20 미안하다!!
	if (!m_pRoom->RoomOut(_socket)) CRoomManager::GetInstance()->RoomOut();
}

void CUser::Ready(SOCKET _socket)
{
	m_pRoom->Ready(_socket);
}

void CUser::Start()
{
	m_pRoom->Start();
}

void CUser::DrawCard(SOCKET _socket, int _card, int _index)
{
	m_pRoom->DrawCard(_socket, _card, _index);
}

void CUser::TakeCard(SOCKET _socket)
{
	m_pRoom->TakeCard(_socket);
}

void CUser::ChoiceColor(SOCKET _socket, int _color)
{
	m_pRoom->ChoiceColor(_socket, _color);
}

void CUser::Victory()
{
	m_pRoom->Victory();
}

// 2022-05-25 수정
void CUser::HandlePacket()
{
	char* tempBuffer = m_buffer;
	unsigned short packetSize = *(unsigned short*)tempBuffer;
	tempBuffer += sizeof(unsigned short);
	unsigned short type = *(unsigned short*)tempBuffer;
	tempBuffer += sizeof(unsigned short);
	
	switch (type)
	{
	case CS_PT_LOGIN:
	{
		CUserManager* pUserManager = CUserManager::GetInstance();

		memcpy(m_name, tempBuffer, sizeof(wchar_t) * NAME_MAX);
		pUserManager->AddUser(this);

		CUserManager::userList_t userList = *pUserManager->GetUserList();

		char sendBuffer[SENDBUFFER];
		char* sendTempBuffer = sendBuffer;

		int listSize = userList.size();

		int len = sizeof(wchar_t) * NAME_MAX;

		*(unsigned short*)sendTempBuffer = 2 + 2 + (len * listSize);
		sendTempBuffer += sizeof(unsigned short);
		*(unsigned short*)sendTempBuffer = CS_PT_LOGIN;
		sendTempBuffer += sizeof(unsigned short);

		std::list<CUser*>::iterator iter = userList.begin();
		for (; iter != userList.end(); iter++)
		{
			memcpy(sendTempBuffer, (*iter)->GetName(), len);
			sendTempBuffer += len;
		}

		int bufferSize = sendTempBuffer - sendBuffer;

		send(m_socket, sendBuffer, bufferSize, 0);
	}
		break;
	case CS_PT_CREATEROOM:
	{
		char sendBuffer[SENDBUFFER];
		char* sendTempBuffer = sendBuffer;
		bool bCreate = true;

		m_image = *(unsigned short*)tempBuffer;
		tempBuffer += sizeof(unsigned short);

		m_pRoom = CRoomManager::GetInstance()->CreateRoom(tempBuffer);
		if (m_pRoom != nullptr)
		{
			m_pRoom->PlayerIn(m_name, m_image, m_socket);
			bCreate = true;
		}
		else bCreate = false;

		int playerCount = m_pRoom->GetPlayerCount();
		int roomSize = sizeof(CRoom::stROOM);
		int userSize = sizeof(CRoom::stUSER) * PLAYER_MAX;

		*(unsigned short*)sendTempBuffer = 2 + 2 + 2 + 2 + roomSize + userSize;
		sendTempBuffer += sizeof(unsigned short);
		*(unsigned short*)sendTempBuffer = CS_PT_CREATEROOM;
		sendTempBuffer += sizeof(unsigned short);
		*(unsigned short*)sendTempBuffer = bCreate;
		sendTempBuffer += sizeof(unsigned short);
		*(unsigned short*)sendTempBuffer = playerCount;
		sendTempBuffer += sizeof(unsigned short);

		memcpy(sendTempBuffer, m_pRoom->GetInfo(), roomSize);
		sendTempBuffer += roomSize;

		memcpy(sendTempBuffer, m_pRoom->GetInRoomUserInfo(), userSize);
		sendTempBuffer += userSize;

		int bufferSize = sendTempBuffer - sendBuffer;

		int sendSize = send(m_socket, sendBuffer, bufferSize, 0);
		if (sendSize < 0)
		{
			//
		}
	}
		break;
	case CS_PT_USERLIST:
	{
		CUserManager::userList_t userList = *CUserManager::GetInstance()->GetUserList();
		char sendBuffer[SENDBUFFER];
		char* sendTempBuffer = sendBuffer;

		int listSize = userList.size();

		int len = sizeof(wchar_t) * NAME_MAX;

		*(unsigned short*)sendTempBuffer = 2 + 2 + (len * listSize);
		sendTempBuffer += sizeof(unsigned short);
		*(unsigned short*)sendTempBuffer = CS_PT_USERLIST;
		sendTempBuffer += sizeof(unsigned short);

		std::list<CUser*>::iterator iter = userList.begin();
		for (; iter != userList.end(); iter++) // 첫 화면에는 15개만
		{
			memcpy(sendTempBuffer, (*iter)->GetName(), len);
			sendTempBuffer += len;
		}

		int bufferSize = sendTempBuffer - sendBuffer;

		send(m_socket, sendBuffer, bufferSize, 0);
	}
		break;
	case CS_PT_ROOMLIST:
	{
		CRoomManager* pRM = CRoomManager::GetInstance();
		CRoomManager::roomList_t* roomList = pRM->GetRoomList();
		char sendBuffer[SENDBUFFER];
		char* sendTempBuffer = sendBuffer;

		int listSize = pRM->GetCount();
		int len = sizeof(CRoom::stROOM);

		*(unsigned short*)sendTempBuffer = 2 + 2 + (len * listSize);
		sendTempBuffer += sizeof(unsigned short);
		*(unsigned short*)sendTempBuffer = CS_PT_ROOMLIST;
		sendTempBuffer += sizeof(unsigned short);

		std::vector<CRoom*>::iterator iter = roomList->begin();
		for (int i = 0; i < listSize; iter++, i++) // 2022-04-25 수정 : 보여지는 화면만 8개(i 값은 추후에 변경)
		{
			memcpy(sendTempBuffer, (*iter)->GetInfo(), len);
			sendTempBuffer += len;
		}

		int bufferSize = sendTempBuffer - sendBuffer;

		send(m_socket, sendBuffer, bufferSize, 0);
	}
		break;
	case CS_PT_INROOM:
	{
		char sendBuffer[SENDBUFFER];
		char* sendTempBuffer = sendBuffer;
		bool bRoomIn = true;
		int playerCount;

		m_pRoom = CRoomManager::GetInstance()->RoomIn(tempBuffer, m_socket);
		if (m_pRoom != nullptr)
		{
			bRoomIn = true;
		}
		else bRoomIn = false;

		if (bRoomIn)
		{
			playerCount = m_pRoom->GetPlayerCount();

			int size = sizeof(CRoom::stUSER) * PLAYER_MAX;

			*(unsigned short*)sendTempBuffer = 2 + 2 + 2 + sizeof(unsigned short) + size;
			sendTempBuffer += sizeof(unsigned short);
			*(unsigned short*)sendTempBuffer = CS_PT_INROOM;
			sendTempBuffer += sizeof(unsigned short);
			*(unsigned short*)sendTempBuffer = bRoomIn;
			sendTempBuffer += sizeof(unsigned short);
			*(unsigned short*)sendTempBuffer = playerCount;
			sendTempBuffer += sizeof(unsigned short);

			memcpy(sendTempBuffer, m_pRoom->GetInRoomUserInfo(), size);

			int bufferSize = sendTempBuffer - sendBuffer + size;

			send(m_socket, sendBuffer, bufferSize, 0);
		}
	}
		break;
	case CS_PT_OUTROOM:
	{
		CRoomManager* pRM = CRoomManager::GetInstance();
		CRoomManager::roomList_t* roomList = pRM->GetRoomList();
		char sendBuffer[SENDBUFFER];
		char* sendTempBuffer = sendBuffer;

		if (!m_pRoom->RoomOut(m_socket)) pRM->RoomOut();

		int listSize = pRM->GetCount();
		int len = sizeof(CRoom::stROOM);

		*(unsigned short*)sendTempBuffer = 2 + 2 + (len * listSize);
		sendTempBuffer += sizeof(unsigned short);
		*(unsigned short*)sendTempBuffer = CS_PT_OUTROOM;
		sendTempBuffer += sizeof(unsigned short);

		std::vector<CRoom*>::iterator iter = roomList->begin();
		for (int i = 0; i < listSize; iter++, i++) // 2022-04-25 수정 : 보여지는 화면만 8개(i 값은 추후에 변경)
		{
			memcpy(sendTempBuffer, (*iter)->GetInfo(), len);
			sendTempBuffer += len;
		}

		int bufferSize = sendTempBuffer - sendBuffer;

		int sendSize = send(m_socket, sendBuffer, bufferSize, 0);
		if (sendSize < 0)
		{

		}
	}
		break;
	case CS_PT_ROOMSTATE:
	{
		char sendBuffer[SENDBUFFER];
		char* sendTempBuffer = sendBuffer;

		int size = sizeof(CRoom::stROOM) + sizeof(CRoom::stUSER) * PLAYER_MAX;

		*(unsigned short*)sendTempBuffer = 2 + 2 + 2 + size;
		sendTempBuffer += sizeof(unsigned short);
		*(unsigned short*)sendTempBuffer = CS_PT_ROOMSTATE;
		sendTempBuffer += sizeof(unsigned short);
		*(unsigned short*)sendTempBuffer = m_pRoom->GetCurrentCard();
		sendTempBuffer += sizeof(unsigned short);
		memcpy(sendTempBuffer, m_pRoom->GetInfo(), sizeof(CRoom::stROOM));
		sendTempBuffer += sizeof(CRoom::stROOM);
		memcpy(sendTempBuffer, m_pRoom->GetInRoomUserInfo(), sizeof(CRoom::stUSER) * PLAYER_MAX);
		sendTempBuffer += sizeof(CRoom::stUSER) * PLAYER_MAX;

		int bufferSize = sendTempBuffer - sendBuffer;

		send(m_socket, sendBuffer, bufferSize, 0);
	}
		break;
	/**case CS_PT_CHATTING:
		Chatting();
		break;
	case CS_PT_READY:
		Ready();
		break;
	case CS_PT_START:
		Start();
		break;
	case CS_PT_DRAWCARD:
		DrawCard();
		break;
	case CS_PT_TAKECARD:
		TakeCard();
		break;
	case CS_PT_CHOISECOLOR:
		ChoiceColor();
		break;
	case CS_PT_VICTORY:
		m_pUser->Victory();
		char sendBuffer[SENDBUFFER];
		char* tempBuffer = sendBuffer;
		*(unsigned short*)tempBuffer = 2 + 2;
		tempBuffer += sizeof(unsigned short);
		*(unsigned short*)tempBuffer = CS_PT_VICTORY;
		tempBuffer += sizeof(unsigned short);

		int size = tempBuffer - sendBuffer;

		send(m_socket, sendBuffer, size, 0);
		break;
	*/
	}
}

void CUser::OnRecv()
{
	HandlePacket();
}