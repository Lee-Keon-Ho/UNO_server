#include "Room.h"
#include <memory>
#include <time.h>

#define PLAYER_MAX 5
#define START_CARD 7
CRoom::CRoom()
{
}

CRoom::CRoom(int num)
{
	srand(time(NULL));

	m_room.number = num;
	m_room.playerCount = 0;
	m_room.state = true;

	memset(m_pPlayers, 0, sizeof(stUSER) * PLAYER_MAX);
	memset(m_card, 1, CARD_MAX);
}

CRoom::~CRoom()
{
	
}

void CRoom::CreateRoom(char* _name)
{
	// 2022-04-26 ¼öÁ¤ : test
	memcpy(m_room.name, _name, ROOM_NAME_MAX * sizeof(wchar_t));
	m_room.playerCount = 1;
}

bool CRoom::RoomOut(SOCKET _socket)
{
	bool bRoom = true;
	if (m_room.playerCount > 0)
	{
		m_room.playerCount--;

		if (m_room.playerCount <= 0)
		{
			memset(m_room.name, 0, ROOM_NAME_MAX * sizeof(wchar_t));
			m_room.playerCount = 0;
			m_room.state = true;
			bRoom = false;
		}
		else
		{
			for (int i = 0; i < PLAYER_MAX; i++)
			{
				if (m_pPlayers[i].socket == _socket)
				{
					memset(&m_pPlayers[i], 0, sizeof(CRoom::stUSER));
					break;
				}
			}
			bRoom = true;
		}
	}
	return bRoom;
}

// createRoom -> playerin
void CRoom::PlayerIn(wchar_t* _name, int _image, SOCKET _socket)
{
	int num = m_room.playerCount - 1;
	m_pPlayers[num].number = m_room.playerCount;
	m_pPlayers[num].boss = true;
	m_pPlayers[num].image = _image;
	m_pPlayers[num].ready = true;
	memcpy(m_pPlayers[num].playerName, _name, sizeof(wchar_t) * USER_NAME_MAX);
	m_pPlayers[num].socket = _socket;
}

bool CRoom::PlayerIn(char* _playerInfo, SOCKET _socket)
{
	if (m_room.playerCount >= PLAYER_MAX) return false;
	m_room.playerCount++;
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (m_pPlayers[i].number == 0)
		{
			m_pPlayers[i].number = i + 1;
			m_pPlayers[i].image = *(unsigned short*)_playerInfo;
			memcpy(m_pPlayers[i].playerName, _playerInfo + sizeof(unsigned short), USER_NAME_MAX * sizeof(wchar_t));
			m_pPlayers[i].socket = _socket;
			m_pPlayers[i].boss = false;
			break;
		}
	}
	return true;
}

void CRoom::Ready(SOCKET _socket)
{
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (m_pPlayers[i].socket == _socket)
		{
			m_pPlayers[i].ready = !m_pPlayers[i].ready;
			break;
		}
	}
}

void CRoom::Start()
{
	int nCard;
	for (int player = 0; player < m_room.playerCount; player++)
	{
		for (int i = 0; i < START_CARD; i++)
		{
			while (true)
			{
				nCard = rand() % 110;
				if (m_card[nCard])
				{
					m_pPlayers[player].card[i] = nCard;
					m_card[nCard] = false;
					break;
				}
			}
		}
	}
}