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

	for (int color = 0; color < COLOR_MAX - 1; color++)
	{
		for (int card = 0; card < CARD_MAX; card++)
		{
			m_Card[(color * CARD_MAX) + card].number = card;
			m_Card[(color * CARD_MAX) + card].color = color;
		}
	}

	for (int color = COLOR_MAX - 1; color < COLOR_MAX; color++)
	{
		for (int card = CARD_MAX - 2, x = 0; card < CARD_MAX; card++, x++)
		{
			m_Card[(color * CARD_MAX) + x].number = card;
			m_Card[(color * CARD_MAX) + x].color = color;
		}
	}

	memset(m_pPlayers, 0, sizeof(stUSER) * PLAYER_MAX);
	memset(m_bCard, 1, CARD_MAX);
}

CRoom::~CRoom()
{
	
}

void CRoom::CreateRoom(char* _name)
{
	// 2022-04-26 수정 : test
	memcpy(m_room.name, _name, ROOM_NAME_MAX * sizeof(wchar_t));
	m_room.playerCount = 1;
	m_room.state = true;
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

			for (int i = 0; i < PLAYER_MAX; i++)
			{
				memset(&m_pPlayers[i], 0, sizeof(CRoom::stUSER));
			}
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
	m_pPlayers[num].turn = true;
}

bool CRoom::PlayerIn(char* _playerInfo, SOCKET _socket)
{
	if (m_room.playerCount >= PLAYER_MAX) return false;
	m_room.playerCount++;
	m_room.state = true;
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (m_pPlayers[i].number == 0)
		{
			m_pPlayers[i].number = i + 1;
			m_pPlayers[i].image = *(unsigned short*)_playerInfo;
			memcpy(m_pPlayers[i].playerName, _playerInfo + sizeof(unsigned short), USER_NAME_MAX * sizeof(wchar_t));
			m_pPlayers[i].socket = _socket;
			m_pPlayers[i].boss = false;
			m_pPlayers[i].turn = false;
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
	for (int player = 0; player < PLAYER_MAX; player++)
	{
		if (m_pPlayers[player].number != 0)
		{
			for (int i = 0; i < START_CARD; i++)
			{
				while (true)
				{
					nCard = rand() % 110;
					if (m_bCard[nCard])
					{
						m_pPlayers[player].card[i] = nCard;
						m_bCard[nCard] = false;
						break;
					}
				}
			}
			m_pPlayers[player].cardCount = START_CARD;
		}
	}

	while (true)
	{
		nCard = rand() % 110;
		if (m_bCard[nCard])
		{
			m_currentCard = nCard; // 여기도 수정이 필요
			m_bCard[nCard] = false;
			break;
		}
	}
	m_room.state = false;
}

void CRoom::DrawCard(SOCKET _socket, int _card, int _index)
{
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (m_pPlayers[i].socket == _socket)
		{
			if (m_Card[_card].color == m_Card[m_currentCard].color)
			{
				m_currentCard = _card;
				for (int x = _index; x < m_pPlayers[i].cardCount; x++)
				{
					m_pPlayers[i].card[x] = m_pPlayers[i].card[x + 1];
				}
				m_pPlayers[i].cardCount--;
				m_pPlayers[i].turn = false;
				i++;
				if (i == m_room.playerCount)
				{
					m_pPlayers[0].turn = true;
				}
				else
				{
					m_pPlayers[i].turn = true;
				}
			}
			else if (Compare(m_Card[_card].number, m_Card[m_currentCard].number))
			{
				m_currentCard = _card;
				for (int x = _index; x < m_pPlayers[i].cardCount; x++)
				{
					m_pPlayers[i].card[x] = m_pPlayers[i].card[x + 1];
				}
				m_pPlayers[i].cardCount--;
				m_pPlayers[i].turn = false;
				i++;
				if (i == PLAYER_MAX)
				{
					m_pPlayers[0].turn = true;
				}
				else
				{
					m_pPlayers[i].turn = true;
				}
			}
			break;
		}
	}
}

bool CRoom::Compare(int _x1, int _x2)
{
	if (_x1 == _x2) return true;
	
	if (_x1 == 1 || _x1 == 10)
	{
		if (_x2 == 1) return true;
		if (_x2 == 10) return true;
	}
	else if (_x1 == 2 || _x1 == 11)
	{
		if (_x2 == 2) return true;
		if (_x2 == 11) return true;
	}
	else if (_x1 == 3 || _x1 == 12)
	{
		if (_x2 == 3) return true;
		if (_x2 == 12) return true;
	}
	else if (_x1 == 4 || _x1 == 13)
	{
		if (_x2 == 4) return true;
		if (_x2 == 13) return true;
	}
	else if (_x1 == 5 || _x1 == 14)
	{
		if (_x2 == 5) return true;
		if (_x2 == 14) return true;
	}
	else if (_x1 == 6 || _x1 == 15)
	{
		if (_x2 == 6) return true;
		if (_x2 == 15) return true;
	}
	else if (_x1 == 7 || _x1 == 16)
	{
		if (_x2 == 7) return true;
		if (_x2 == 16) return true;
	}
	else if (_x1 == 8 || _x1 == 17)
	{
		if (_x2 == 8) return true;
		if (_x2 == 17) return true;
	}
	else if (_x1 == 9 || _x1 == 18)
	{
		if (_x2 == 9) return true;
		if (_x2 == 18) return true;
	}
	else
	{
		return false;
	}
	return false;
}