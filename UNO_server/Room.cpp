#include "Room.h"
#include <memory>
#include <time.h>

#define PLAYER_MAX 5
#define START_CARD 7
CRoom::CRoom()
{
}

CRoom::CRoom(int num) : m_bTurn(true), m_nTakeCardCount(1)
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

			for (int i = 0; i < PLAYER_MAX; i++)
			{
				if (m_pPlayers[i].number != 0)
				{
					m_pPlayers[i].boss = true;
					m_pPlayers[i].ready = true;
					m_pPlayers[i].turn = true;
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
		if (m_Card[nCard].number < 19)
		{
			m_nColor = m_Card[nCard].color;
			if (m_bCard[nCard])
			{
				m_currentCard = nCard; 
				m_bCard[nCard] = false;
				break;
			}
		}
	}
	m_room.state = false;
}

void CRoom::DrawCard(SOCKET _socket, int _card, int _userCardindex)
{
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (m_pPlayers[i].socket == _socket)
		{
			stCARD userCard = m_Card[_card];
			stCARD currentCard = m_Card[m_currentCard];
			bool bOk = false;

			if (userCard.color == m_nColor)
			{
				if (currentCard.number < 19)
				{
					if (userCard.number == 19 || userCard.number == 22)
					{
						m_nTakeCardCount = 2;
					}
					else if (userCard.number == 20)
					{
						m_bTurn = false;
					}
					else if (userCard.number == 25)
					{
						// 색상 변경 구현
					}
					else if (userCard.number == 26)
					{
						m_nTakeCardCount = 4;
					}
					m_currentCard = _card;
					bOk = true;
				}
				else if (currentCard.number == 19 || currentCard.number == 22) // +2
				{
					if (m_nTakeCardCount == 1)
					{
						m_currentCard = _card;
						bOk = true;
					}
					else if (userCard.number == 19 || userCard.number == 22) // +2
					{
						m_nTakeCardCount += 2;
						m_currentCard = _card;
						bOk = true;
					}
					else if (userCard.number == 21 || userCard.number == 24) // stop
					{
						m_nTakeCardCount = 1;
						m_currentCard = _card;
						bOk = true;
					}
					else if (userCard.number == 26) // +4
					{
						m_nTakeCardCount += 4;
						m_currentCard = _card;
						bOk = true;
					}
				}
				else if (currentCard.number == 20 || currentCard.number == 23)
				{
					m_currentCard = _card;
					m_bTurn = !m_bTurn;
					bOk = true;
				}
				else if (currentCard.number == 21 || currentCard.number == 24)
				{
					m_currentCard = _card;
					bOk = true;
				}
				else if (currentCard.number == 25)
				{
					m_currentCard = _card;
					// 색상 변경 구현
					bOk = true;
				}
				else if (currentCard.number == 26)
				{
					if (userCard.number == 26)
					{
						m_nTakeCardCount += 4;

					}
					else if (userCard.number == 21 || userCard.number == 24)
					{
						m_nTakeCardCount = 1;
					}
					m_currentCard = _card;
					m_bTurn = true;
					bOk = true;
				}
			}
			else if (Compare(userCard.number, currentCard.number))
			{
				m_nColor = userCard.color;
				m_currentCard = _card;
				bOk = true;
			}
			else bOk = false;

			if (bOk)
			{
				for (int x = _userCardindex; x < m_pPlayers[i].cardCount; x++)
				{
					m_pPlayers[i].card[x] = m_pPlayers[i].card[x + 1];
				}
				m_pPlayers[i].cardCount--;
				m_pPlayers[i].turn = false;

				NextTurn(m_bTurn, i);
				m_bCard[_card] = true;
			}
			break;
		}
	}
}

void CRoom::TakeCard(SOCKET _socket)
{
	int nCard;
	for (int index = 0; index < PLAYER_MAX; index++)
	{
		if (m_pPlayers[index].socket == _socket)
		{	
			for (int i = 0; i < m_nTakeCardCount; i++)
			{
				int cardCount = m_pPlayers[index].cardCount;
				while (true)
				{
					nCard = rand() % 110;
					if (m_bCard[nCard])
					{
						m_pPlayers[index].card[cardCount] = nCard;
						m_bCard[nCard] = false;
						m_pPlayers[index].turn = false;
						break;
					}
				}
				m_pPlayers[index].cardCount++;
				if (m_pPlayers[index].cardCount > 20)
				{
					for (int count = 0; count < 20; i++)
					{
						nCard = m_pPlayers[index].card[count];
						m_bCard[nCard] = true;
					}
					// player game over
				}
			}
			m_nTakeCardCount = 1;
			NextTurn(m_bTurn, index);
		}
	}
}

bool CRoom::Compare(int _userCard, int _currentCard)
{
	if (_userCard == _currentCard) return true;

	if (_currentCard == 1 || _currentCard == 10)
	{
		if (_userCard == 1) return true;
		if (_userCard == 10) return true;
	}
	else if (_currentCard == 2 || _currentCard == 11)
	{
		if (_userCard == 2) return true;
		if (_userCard == 11) return true;
	}
	else if (_currentCard == 3 || _currentCard == 12)
	{
		if (_userCard == 3) return true;
		if (_userCard == 12) return true;
	}
	else if (_currentCard == 4 || _currentCard == 13)
	{
		if (_userCard == 4) return true;
		if (_userCard == 13) return true;
	}
	else if (_currentCard == 5 || _currentCard == 14)
	{
		if (_userCard == 5) return true;
		if (_userCard == 14) return true;
	}
	else if (_currentCard == 6 || _currentCard == 15)
	{
		if (_userCard == 6) return true;
		if (_userCard == 15) return true;
	}
	else if (_currentCard == 7 || _currentCard == 16)
	{
		if (_userCard == 7) return true;
		if (_userCard == 16) return true;
	}
	else if (_currentCard == 8 || _currentCard == 17)
	{
		if (_userCard == 8) return true;
		if (_userCard == 17) return true;
	}
	else if (_currentCard == 9 || _currentCard == 18)
	{
		if (_userCard == 9) return true;
		if (_userCard == 18) return true;
	}
	else if(_currentCard == 19 || _currentCard == 22)
	{
		if (_userCard == 19) return true;
		if (_userCard == 21) return true;
		if (_userCard == 22) return true;
		if (_userCard == 24) return true;
		if (_userCard == 26) return true;
	}
	else if (_currentCard == 20 || _currentCard == 23)
	{
		if (_userCard == 20 || _userCard == 23)
		{
			m_bTurn = !m_bTurn;
			return true;
		}
	}
	else if (_currentCard == 21 || _currentCard == 24)
	{
		if (_userCard == 21) return true;
		if (_userCard == 24) return true;
	}
	else if (_currentCard == 25)
	{
		if (_userCard == 25) return true;
		if (m_Card[_userCard].color == m_nColor) return true;
	}
	else if (_currentCard == 26)
	{
		if (_userCard == 21) return true;
		if (_userCard == 24) return true;
		if (_userCard == 26) return true;
	}
	return false;
}

void CRoom::NextTurn(bool _turn, int _i)
{
	if (_turn)
	{
		while (true)
		{
			++_i;
			if (_i >= PLAYER_MAX) _i = 0;
			if (m_pPlayers[_i].number != 0)
			{
				m_pPlayers[_i].turn = true;
				break;
			}
		}
	}
	else
	{
		while (true)
		{
			--_i;
			if (_i < 0) _i = PLAYER_MAX - 1;
			if (m_pPlayers[_i].number != 0)
			{
				m_pPlayers[_i].turn = true;
				break;
			}
		}
	}
}
