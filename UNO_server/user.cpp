#include "User.h"
#include "PacketType.h"
#include "userManager.h"
#include "roomManager.h"
#include <stdio.h>
#include <string.h>

#define SENDBUFFER 2000
#define CHATBUFFER 64
#define START_CARD 7

CUser::CUser()
{
	
}

CUser::CUser(SOCKET _socket, SOCKADDR_IN& _addr)
	: CSession(_socket, _addr)
{
	m_MyInfo.boss = false;
	m_MyInfo.ready = false;
	m_MyInfo.turn = false;
	m_MyInfo.choiceColor = false;
	m_MyInfo.cardCount = -1;
	m_MyInfo.number = 0;
}

CUser::~CUser()
{
	CSession::~CSession();
}

void CUser::Setting(int _number, bool _boss, bool _ready, bool _turn)
{
	m_MyInfo.number = _number;
	m_MyInfo.boss = _boss;
	m_MyInfo.ready = _ready;
	m_MyInfo.turn = _turn;
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
		LogIn(tempBuffer);
		break;
	case CS_PT_CREATEROOM:
		CreateRoom(tempBuffer);
		if(m_pRoom != nullptr) RoomInOutChat(L"님이 입장하였습니다.");
		break;
	case CS_PT_USERLIST:
		UserList();
		break;
	case CS_PT_ROOMLIST:
		RoomList();
		break;
	case CS_PT_INROOM:
		RoomIn(tempBuffer);
		if (m_pRoom != nullptr) RoomInOutChat(L"님이 입장하였습니다.");
		break;
	case CS_PT_OUTROOM:
		RoomOut();
		if (m_pRoom != nullptr) RoomInOutChat(L"님이 퇴장하였습니다.");
		break;
	case CS_PT_ROOMSTATE:
		RoomState();
		break;
	case CS_PT_CHATTING:
		Chatting();
		break;
	case CS_PT_READY:
		m_MyInfo.ready = *(unsigned short*)tempBuffer;
		break;
	case CS_PT_START:
		GameStart();
		break;
	case CS_PT_DRAWCARD:
		UserDrawCard(tempBuffer);
		break;
	case CS_PT_TAKECARD:
		m_pRoom->TakeCard(this);
		break;
	case CS_PT_CHOISECOLOR:
		ChoiseColor(tempBuffer);
		break;
	case CS_PT_RESET:
		GameReset();
		break;
	case CS_PT_LOGOUT:
		CUserManager::GetInstance()->Remove(this);
		break;
	case CS_PT_QUICKROOM:
		QuickRoomIn();
		if (m_pRoom != nullptr) RoomInOutChat(L"님이 입장하였습니다.");
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
	m_MyInfo.cardCount = START_CARD;
}

void CUser::DrawCard(int _cardIndex)
{
	for (int i = _cardIndex; i < m_MyInfo.cardCount; i++)
	{
		m_card[i] = m_card[i + 1];
	}
	m_MyInfo.cardCount--;
	m_MyInfo.turn = false;
}

void CUser::DrawChoiceCard(int _cardIndex)
{
	for (int i = _cardIndex; i < m_MyInfo.cardCount; i++)
	{
		m_card[i] = m_card[i + 1];
	}
	m_MyInfo.cardCount--;
	m_MyInfo.turn = false;
	m_MyInfo.choiceColor = true;
}

bool CUser::TakeCard(int _card)
{
	if (m_MyInfo.cardCount < USER_CARD_MAX)
	{
		m_card[m_MyInfo.cardCount] = _card;
	}
	m_MyInfo.cardCount++;
	if (m_MyInfo.cardCount < GAME_OVER) return true;
	else return false;
}

bool CUser::Victory()
{
	if (m_MyInfo.cardCount <= 0) return true;
	else return false;
}

void CUser::GameOver(bool* _bCard)
{
	int nCard;
	for (int i = 0; i < m_MyInfo.cardCount; i++)
	{
		nCard = m_card[i];
		_bCard[i] = true;
	}
}

void CUser::Boss()
{
	m_MyInfo.boss = true;
	m_MyInfo.ready = true;
	m_MyInfo.turn = true;
}

void CUser::Reset()
{
	if (m_MyInfo.boss)
	{
		m_MyInfo.ready = true;
		m_MyInfo.turn = true;
		
	}
	else
	{
		m_MyInfo.ready = false;
		m_MyInfo.turn = false;
	}
	m_MyInfo.choiceColor = false;
	m_MyInfo.cardCount = -1;
}

void CUser::LogIn(char* _info)
{
	CUserManager* pUserManager = CUserManager::GetInstance();

	memcpy(m_MyInfo.name, _info, sizeof(wchar_t) * NAME_MAX);
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

void CUser::CreateRoom(char* _info)
{
	char* tempBuffer = _info;
	char sendBuffer[SENDBUFFER];
	char* sendTempBuffer = sendBuffer;
	bool bCreate = true;

	m_MyInfo.image = *(unsigned short*)tempBuffer;
	tempBuffer += sizeof(unsigned short);

	m_pRoom = CRoomManager::GetInstance()->CreateRoom(tempBuffer);
	if (m_pRoom != nullptr)
	{
		m_pRoom->PlayerIn(this);
		bCreate = true;
	}
	else bCreate = false;

	if (bCreate)
	{
		int playerCount = m_pRoom->GetPlayerCount();
		int roomSize = sizeof(CRoom::stROOM);
		//int userSize = sizeof(CRoom::stUSER) * PLAYER_MAX; // 수정이 필요한 부분

		int userSize = (sizeof(m_MyInfo) * m_pRoom->GetPlayerCount());

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
			if (temp[i] != nullptr)
			{
				memcpy(sendTempBuffer, temp[i]->GetGameInfo(), sizeof(m_MyInfo));
				sendTempBuffer += sizeof(m_MyInfo);
			}
		}

		int bufferSize = sendTempBuffer - sendBuffer;

		int sendSize = send(m_socket, sendBuffer, bufferSize, 0);
	}
}

void CUser::UserList()
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

void CUser::RoomList()
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

void CUser::RoomIn(char* _info)
{
	char* tempBuffer = _info;
	char sendBuffer[SENDBUFFER];
	char* sendTempBuffer = sendBuffer;
	bool bRoomIn = true;
	int playerCount;

	m_pRoom = CRoomManager::GetInstance()->RoomIn(*(unsigned short*)tempBuffer - 1);
	tempBuffer += sizeof(unsigned short);

	m_MyInfo.image = *(unsigned short*)tempBuffer;
	if (m_pRoom != nullptr)
	{
		m_pRoom->PlayerIn(this);
		bRoomIn = true;
	}
	else bRoomIn = false;

	if (bRoomIn)
	{
		playerCount = m_pRoom->GetPlayerCount();

		int userSize = sizeof(CUser::stUserInfo) * playerCount;
		int roomSize = sizeof(CRoom::stROOM);

		*(unsigned short*)sendTempBuffer = 2 + 2 + 2 + roomSize + userSize;
		sendTempBuffer += sizeof(unsigned short);
		*(unsigned short*)sendTempBuffer = CS_PT_INROOM;
		sendTempBuffer += sizeof(unsigned short);
		*(unsigned short*)sendTempBuffer = bRoomIn;
		sendTempBuffer += sizeof(unsigned short);

		memcpy(sendTempBuffer, m_pRoom->GetInfo(), roomSize);
		sendTempBuffer += roomSize;

		CUser** temp = m_pRoom->GetUser();
		for (int i = 0; i < PLAYER_MAX; i++)
		{
			if (temp[i] != nullptr)
			{
				memcpy(sendTempBuffer, temp[i]->GetGameInfo(), sizeof(CUser::stUserInfo));
				sendTempBuffer += sizeof(CUser::stUserInfo);
			}
		}

		int bufferSize = sendTempBuffer - sendBuffer;

		send(m_socket, sendBuffer, bufferSize, 0);
	}
}

void CUser::RoomOut()
{
	CRoomManager* pRM = CRoomManager::GetInstance();
	CRoomManager::roomList_t* roomList = pRM->GetRoomList();
	char sendBuffer[SENDBUFFER];
	char* sendTempBuffer = sendBuffer;

	if (!m_pRoom->RoomOut(m_socket)) pRM->RoomOut();

	*(unsigned short*)sendTempBuffer = 2 + 2;
	sendTempBuffer += sizeof(unsigned short);
	*(unsigned short*)sendTempBuffer = CS_PT_OUTROOM;
	sendTempBuffer += sizeof(unsigned short);

	int bufferSize = sendTempBuffer - sendBuffer;

	int sendSize = send(m_socket, sendBuffer, bufferSize, 0);
	if (sendSize < 0)
	{

	}
}

void CUser::RoomState()
{
	char sendBuffer[SENDBUFFER];
	char* sendTempBuffer = sendBuffer;

	int size = sizeof(CRoom::stROOM) + (sizeof(int) * USER_CARD_MAX) + (sizeof(m_MyInfo) * m_pRoom->GetPlayerCount());

	*(unsigned short*)sendTempBuffer = 2 + 2 + 2 + size;
	sendTempBuffer += sizeof(unsigned short);
	*(unsigned short*)sendTempBuffer = CS_PT_ROOMSTATE;
	sendTempBuffer += sizeof(unsigned short);
	*(unsigned short*)sendTempBuffer = m_pRoom->GetCurrentCard();
	sendTempBuffer += sizeof(unsigned short);
	memcpy(sendTempBuffer, m_card, sizeof(int) * USER_CARD_MAX);
	sendTempBuffer += sizeof(int) * USER_CARD_MAX;
	memcpy(sendTempBuffer, m_pRoom->GetInfo(), sizeof(CRoom::stROOM));
	sendTempBuffer += sizeof(CRoom::stROOM);

	CUser** temp = m_pRoom->GetUser();
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (temp[i] != nullptr)
		{
			memcpy(sendTempBuffer, temp[i]->GetGameInfo(), sizeof(m_MyInfo));
			sendTempBuffer += sizeof(m_MyInfo);
		}
	}

	int bufferSize = sendTempBuffer - sendBuffer;

	send(m_socket, sendBuffer, bufferSize, 0);
}

void CUser::Chatting()
{
	char buffer[CHATBUFFER];
	memset(buffer, 0, CHATBUFFER);
	char* tempBuffer = buffer;

	unsigned short packetSize = *(unsigned short*)m_buffer;

	int size = wcslen(m_MyInfo.name) * sizeof(wchar_t);
	memcpy(tempBuffer, m_MyInfo.name, size);
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
		if (m_pRoom->GetUser()[i] != nullptr)
		{
			int sendSize = send(m_pRoom->GetUser()[i]->GetSocket(), sendBuffer, sendLen, 0);
			if (sendSize < 0)
			{
				break;
			}
		}

	}
}

void CUser::GameStart()
{
	m_pRoom->Start();
	for (int i = 0; i < m_pRoom->GetPlayerCount(); i++)
	{
		char sendBuffer[BUFFER_MAX];
		char* sendTempBuffer = sendBuffer;

		int cardSize = sizeof(int) * USER_CARD_MAX;

		*(unsigned short*)sendTempBuffer = 2 + 2 + cardSize;
		sendTempBuffer += sizeof(unsigned short);
		*(unsigned short*)sendTempBuffer = CS_PT_START;
		sendTempBuffer += sizeof(unsigned short);
		memcpy(sendTempBuffer, m_pRoom->GetUserCard(i), cardSize);
		sendTempBuffer += cardSize;

		int sendSize = sendTempBuffer - sendBuffer;

		send(m_pRoom->GetUserSocket(i), sendBuffer, sendSize, 0);
	}
}

void CUser::UserDrawCard(char* _cardInfo)
{
	char* tempBuffer = _cardInfo;
	int cardNum = *(unsigned short*)tempBuffer;
	tempBuffer += sizeof(unsigned short);
	int index = *(unsigned short*)tempBuffer;
	m_pRoom->DrawCard(this, cardNum, index);
}

void CUser::ChoiseColor(char* _color)
{
	int color = *(unsigned short*)_color;

	m_pRoom->ChoiceColor(this, color);
}

void CUser::GameReset()
{
	m_pRoom->Reset();

	char sendBuffer[SENDBUFFER];
	char* sendTempBuffer = sendBuffer;
	*(unsigned short*)sendTempBuffer = 2 + 2;
	sendTempBuffer += sizeof(unsigned short);
	*(unsigned short*)sendTempBuffer = CS_PT_RESET;
	sendTempBuffer += sizeof(unsigned short);

	int size = sendTempBuffer - sendBuffer;

	send(m_socket, sendBuffer, size, 0);
}

void CUser::RoomInOutChat(const wchar_t* _str)
{
	char buffer[CHATBUFFER];
	memset(buffer, 0, CHATBUFFER);
	char* tempBuffer = buffer;

	int strLen = wcslen(_str);

	unsigned short packetSize = *(unsigned short*)m_buffer;

	int size = wcslen(m_MyInfo.name) * sizeof(wchar_t);
	memcpy(tempBuffer, m_MyInfo.name, size);
	tempBuffer += size;
	memcpy(tempBuffer, _str, strLen * sizeof(wchar_t));
	tempBuffer += strLen * sizeof(wchar_t);

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
		if (m_pRoom->GetUser()[i] != nullptr)
		{
			int sendSize = send(m_pRoom->GetUser()[i]->GetSocket(), sendBuffer, sendLen, 0);
			if (sendSize < 0)
			{
				break;
			}
		}

	}
}

void CUser::QuickRoomIn()
{
	char sendBuffer[SENDBUFFER];
	char* sendTempBuffer = sendBuffer;
	bool bRoomIn = true;
	int playerCount;

	m_pRoom = CRoomManager::GetInstance()->QuickRoomIn();
	if (m_pRoom != nullptr)
	{
		m_pRoom->PlayerIn(this);
		bRoomIn = true;
	}
	else bRoomIn = false;

	if (bRoomIn)
	{
		playerCount = m_pRoom->GetPlayerCount();

		int userSize = sizeof(CUser::stUserInfo) * playerCount;
		int roomSize = sizeof(CRoom::stROOM);

		*(unsigned short*)sendTempBuffer = 2 + 2 + 2 + roomSize + userSize;
		sendTempBuffer += sizeof(unsigned short);
		*(unsigned short*)sendTempBuffer = CS_PT_INROOM;
		sendTempBuffer += sizeof(unsigned short);
		*(unsigned short*)sendTempBuffer = bRoomIn;
		sendTempBuffer += sizeof(unsigned short);

		memcpy(sendTempBuffer, m_pRoom->GetInfo(), roomSize);
		sendTempBuffer += roomSize;

		CUser** temp = m_pRoom->GetUser();
		for (int i = 0; i < PLAYER_MAX; i++)
		{
			if (temp[i] != nullptr)
			{
				memcpy(sendTempBuffer, temp[i]->GetGameInfo(), sizeof(CUser::stUserInfo));
				sendTempBuffer += sizeof(CUser::stUserInfo);
			}
		}

		int bufferSize = sendTempBuffer - sendBuffer;

		send(m_socket, sendBuffer, bufferSize, 0);
	}
}
