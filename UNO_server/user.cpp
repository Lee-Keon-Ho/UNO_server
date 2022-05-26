#include "User.h"
#include "PacketType.h"
#include "userManager.h"
#include "roomManager.h"
#include <stdio.h>
#include <string.h>

#define SENDBUFFER 2000
#define CHATBUFFER 64

CUser::CUser()
{
	
}

CUser::CUser(SOCKET _socket, SOCKADDR_IN& _addr)
	: CSession(_socket, _addr)
{
	m_MyGameInfo.boss = false;
	m_MyGameInfo.ready = false;
	m_MyGameInfo.turn = false;
	m_MyGameInfo.choiceColor = false;
	m_MyGameInfo.cardCount = -1;
	m_MyGameInfo.number = 0;
}

CUser::~CUser()
{
	CSession::~CSession();
}

void CUser::Setting(int _number, bool _boss, bool _ready, bool _turn)
{
	m_MyGameInfo.number = _number;
	m_MyGameInfo.boss = _boss;
	m_MyGameInfo.ready = _ready;
	m_MyGameInfo.turn = _turn;
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

		memcpy(m_MyInfo.name, tempBuffer, sizeof(wchar_t) * NAME_MAX);
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

		m_MyInfo.image = *(unsigned short*)tempBuffer;
		tempBuffer += sizeof(unsigned short);

		m_pRoom = CRoomManager::GetInstance()->CreateRoom(tempBuffer);
		if (m_pRoom != nullptr)
		{
			m_pRoom->PlayerIn(this);
			//m_pRoom->PlayerIn(m_name, m_image, m_socket);
			bCreate = true;
		}
		else bCreate = false;

		int playerCount = m_pRoom->GetPlayerCount();
		int roomSize = sizeof(CRoom::stROOM);
		//int userSize = sizeof(CRoom::stUSER) * PLAYER_MAX; // 수정이 필요한 부분

		int userSize = ((sizeof(WCHAR) * NAME_MAX) + 2 + 2 + 2 + 2 + 2 + 2 + 2) * m_pRoom->GetPlayerCount();

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

		CUser** temp = m_pRoom->GetUser();
		for (int i = 0; i < PLAYER_MAX; i++)
		{
			temp[i]->GetGameInfo();
		}
		//memcpy(sendTempBuffer, m_pRoom->GetInRoomUserInfo(), userSize);
		//sendTempBuffer += userSize;

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
	case CS_PT_CHATTING:
	{
		char buffer[CHATBUFFER];
		memset(buffer, 0, CHATBUFFER);
		char* tempBuffer = buffer;

		unsigned short packetSize = *(unsigned short*)m_buffer;

		int size = wcslen(m_name) * sizeof(wchar_t);
		memcpy(tempBuffer, m_name, size);
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

		for (int i = 0; i < PLAYER_MAX; i++)
		{
			if (m_pRoom->GetInRoomUserInfo()[i].socket != 0)
			{
				int sendSize = send(m_pRoom->GetInRoomUserInfo()[i].socket, sendBuffer, sendLen, 0);
				if (sendSize < 0)
				{
					break;
				}
			}

		}
	}
		break;
	case CS_PT_READY:
		m_ready;
		break;
	case CS_PT_START:
		m_pRoom->Start();
		break;
	case CS_PT_DRAWCARD:
	{
		int cardNum = *(unsigned short*)tempBuffer;
		tempBuffer += sizeof(unsigned short);
		int index = *(unsigned short*)tempBuffer;
		m_pRoom->DrawCard(m_socket, cardNum, index);

		//RoomState();
	}
		break;
	case CS_PT_TAKECARD:
		m_pRoom->TakeCard(m_socket);
		break;
	case CS_PT_CHOISECOLOR:
	{
		int color = *(unsigned short*)tempBuffer;
		m_pRoom->ChoiceColor(m_socket, color);

		//RoomState();
	}
		break;
	case CS_PT_VICTORY:
	{
		m_pRoom->Victory();
		char sendBuffer[SENDBUFFER];
		char* sendTempBuffer = sendBuffer;
		*(unsigned short*)sendTempBuffer = 2 + 2;
		sendTempBuffer += sizeof(unsigned short);
		*(unsigned short*)sendTempBuffer = CS_PT_VICTORY;
		sendTempBuffer += sizeof(unsigned short);

		int size = sendTempBuffer - sendBuffer;

		send(m_socket, sendBuffer, size, 0);
	}
		break;
	}
}

void CUser::OnRecv()
{
	HandlePacket();
}

void CUser::Start(bool* _bCard)
{
	int nCard;
	for (int i = 0; i < START_CARD; i++)
	{
		while (true)
		{
			nCard = rand() % CARD_ALL;
			if (_bCard[nCard])
			{
				m_card[i] = nCard;
				_bCard[nCard] = false;
				break;
			}
		}
	}
	m_cardCount = START_CARD;
}

void CUser::DrawCard(int _cardIndex)
{
	for (int i = _cardIndex; i < m_cardCount; i++)
	{
		m_card[i] = m_card[i + 1];
	}
	m_cardCount--;
	m_turn = false;
}

void CUser::DrawChoiceCard(int _cardIndex)
{
	for (int i = _cardIndex; i < m_cardCount; i++)
	{
		m_card[i] = m_card[i + 1];
	}
	m_cardCount--;
	m_turn = true;
	m_choiceColor = true;
}

bool CUser::TakeCard(int _card)
{
	m_card[m_cardCount] = _card;
	m_cardCount++;
	if (m_cardCount < GAME_OVER) return true;
	else return false;
}

bool CUser::Victory()
{
	if (m_cardCount <= 0) return true;
	else return false;
}

void CUser::GameOver(bool* _bCard)
{
	int nCard;
	for (int i = 0; i < m_cardCount; i++)
	{
		nCard = m_card[i];
		_bCard[i] = true;
	}
}

void CUser::Boss()
{
	m_boss = true;
	m_ready = true;
	m_turn = true;
}
