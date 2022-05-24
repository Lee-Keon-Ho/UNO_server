#include "Room.h"
#include <memory>
#include <time.h>

#define PLAYER_MAX 5
#define START_CARD 2

#define COLOR_RED 0
#define COLOR_GREEN 1
#define COLOR_BLUE 2
#define COLOR_YELLOW 3

CRoom::CRoom()
{
}

CRoom::CRoom(int num) : m_bTurn(true), m_nTakeCardCount(1)
{
	srand(time(NULL));

	m_room.number = num;
	m_room.playerCount = 0;
	m_room.state = true;
	m_room.victory = false;

	//for (int color = 0; color < COLOR_MAX - 1; color++)
	for (int color = 0; color < COLOR_MAX; color++)
	{
		for (int card = 0; card < CARD_MAX; card++)
		{
			m_Card[(color * CARD_MAX) + card].number = card;
			m_Card[(color * CARD_MAX) + card].color = color;
		}
	}

	/*for (int color = COLOR_MAX - 1; color < COLOR_MAX; color++)
	{
		for (int card = CARD_MAX - 2, x = 0; card < CARD_MAX; card++, x++)
		{
			m_Card[(color * CARD_MAX) + x].number = card;
			m_Card[(color * CARD_MAX) + x].color = color;
		}
	}*/

	memset(m_pPlayers, 0, sizeof(stUSER) * PLAYER_MAX);
	memset(m_bCard, 1, CARD_MAX);
}

CRoom::~CRoom()
{
	
}

void CRoom::CreateRoom(char* _name)
{
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
	m_pPlayers[num].cardCount = -1;
	memcpy(m_pPlayers[num].playerName, _name, sizeof(wchar_t) * USER_NAME_MAX);
	m_pPlayers[num].socket = _socket;
	m_pPlayers[num].turn = true;
	m_pPlayers[num].choiceColor = false;
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
			m_pPlayers[i].cardCount = -1;
			m_pPlayers[i].socket = _socket;
			m_pPlayers[i].boss = false;
			m_pPlayers[i].turn = false;
			m_pPlayers[i].choiceColor = false;
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
					nCard = rand() % CARD_ALL;
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
		nCard = rand() % CARD_ALL;
		if (m_Card[nCard].number < GAME_OVER)
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
				if (ColorCompare(userCard.number, currentCard.number))
				{
					m_currentCard = _card;
					bOk = true;
				}
				else if (userCard.number == 25)
				{
					for (int x = _userCardindex; x < m_pPlayers[i].cardCount; x++)
					{
						m_pPlayers[i].card[x] = m_pPlayers[i].card[x + 1];
					}
					m_pPlayers[i].cardCount--;
					m_pPlayers[i].turn = true;
					m_pPlayers[i].choiceColor = true;

					m_bCard[_card] = true;
					bOk = false;
				}
				else bOk = false;
			}
			else if (NumCompare(userCard.number, currentCard.number))
			{
				m_nColor = userCard.color;
				m_currentCard = _card;
				bOk = true;
			}
			else if (userCard.number == 25)
			{
				for (int x = _userCardindex; x < m_pPlayers[i].cardCount; x++)
				{
					m_pPlayers[i].card[x] = m_pPlayers[i].card[x + 1];
				}
				m_pPlayers[i].cardCount--;
				m_pPlayers[i].turn = true;
				m_pPlayers[i].choiceColor = true;

				m_bCard[_card] = true;
				bOk = false;
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

				if (m_pPlayers[i].cardCount <= 0)
				{
					m_room.victory = true;
				}

				m_bCard[_card] = true;
				NextTurn(m_bTurn, i);
			}
			else
			{
				if (m_pPlayers[i].cardCount <= 0)
				{
					m_room.victory = true;
				}
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
					nCard = rand() % CARD_ALL;
					if (m_bCard[nCard])
					{
						m_pPlayers[index].card[cardCount] = nCard;
						m_bCard[nCard] = false;
						m_pPlayers[index].turn = false;
						break;
					}
				}
				m_pPlayers[index].cardCount++;
				if (m_pPlayers[index].cardCount >= GAME_OVER)
				{
					for (int count = 0; count < GAME_OVER; count++)
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

void CRoom::ChoiceColor(SOCKET _socket, int _color)
{
	m_currentCard = 25 + (_color * 27);
	m_nColor = _color;
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (m_pPlayers[i].socket == _socket)
		{
			m_pPlayers[i].choiceColor = false;
			m_pPlayers[i].turn = false;
			NextTurn(m_bTurn, i);
		}
	}
}

void CRoom::Victory()
{
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (m_pPlayers[i].number != 0)
		{
			if (m_pPlayers[i].boss)
			{
				m_pPlayers[i].ready = true;
				m_pPlayers[i].turn = true;
				m_pPlayers[i].choiceColor = false;
				m_pPlayers[i].cardCount = -1;
			}
			else
			{
				m_pPlayers[i].ready = false;
				m_pPlayers[i].turn = false;
				m_pPlayers[i].choiceColor = false;
				m_pPlayers[i].cardCount = -1;
			}
		}
	}
	m_room.victory = false;
	m_room.state = true;
}

bool CRoom::NumCompare(int _userCard, int _currentCard)
{
	if (_userCard == _currentCard)
	{
		if (m_nTakeCardCount == 1)
		{
			if (_userCard == 19 || _userCard == 22)
			{
				m_nTakeCardCount = 2;
			}
			else if (_userCard == 20 || _userCard == 23)
			{
				m_bTurn = !m_bTurn;
			}
			else if (_userCard == 25)
			{
				return false;
			}
			else if (_userCard == 26)
			{
				m_nTakeCardCount = 4;
			}
		}
		else if (_userCard == 19 || _userCard == 22)
		{
			m_nTakeCardCount += 2;
		}
		else if (_userCard == 26)
		{
			m_nTakeCardCount += 4;
		}
		return true;
	}

	if (_currentCard == 1 || _currentCard == 10)
	{
		if (_userCard == 1 || _userCard == 10) return true;
	}
	else if (_currentCard == 2 || _currentCard == 11)
	{
		if (_userCard == 2 || _userCard == 11) return true;
	}
	else if (_currentCard == 3 || _currentCard == 12)
	{
		if (_userCard == 3 || _userCard == 12) return true;
	}
	else if (_currentCard == 4 || _currentCard == 13)
	{
		if (_userCard == 4 || _userCard == 13) return true;
	}
	else if (_currentCard == 5 || _currentCard == 14)
	{
		if (_userCard == 5 || _userCard == 14) return true;
	}
	else if (_currentCard == 6 || _currentCard == 15)
	{
		if (_userCard == 6 || _userCard == 15) return true;
	}
	else if (_currentCard == 7 || _currentCard == 16)
	{
		if (_userCard == 7 || _userCard == 16) return true;
	}
	else if (_currentCard == 8 || _currentCard == 17)
	{
		if (_userCard == 8 || _userCard == 17) return true;
	}
	else if (_currentCard == 9 || _currentCard == 18)
	{
		if (_userCard == 9 || _userCard == 18) return true;
	}
	else if(_currentCard == 19 || _currentCard == 22)
	{
		if (_userCard == 19 || _userCard == 22) return true;
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
		if (_userCard == 21 || _userCard == 24) return true;
	}
	else if (_currentCard == 25)
	{
		if (_userCard == 25) return false;
	}
	else if (_currentCard == 26)
	{
		if (_userCard == 26) return true;
	}
	return false;
}

bool CRoom::ColorCompare(int _userCard, int _currentCard)
{
	if (_currentCard < 19) // number
	{
		if (_userCard == 19 || _userCard == 22)
		{
			m_nTakeCardCount = 2;
		}
		else if (_userCard == 20 || _userCard == 23)
		{
			m_bTurn = !m_bTurn; // ¹Ý´ë·Î
		}
		else if (_userCard == 25)
		{
			return false;
		}
		else if (_userCard == 26)
		{
			m_nTakeCardCount = 4;
		}
		return true;
	}
	else if (_currentCard == 19 || _currentCard == 22) // +2
	{
		if (m_nTakeCardCount == 1)
		{
			return true;
		}
		if (_userCard == 19 || _userCard == 22) // +2
		{
			m_nTakeCardCount += 2;
			return true;
		}
		else if (_userCard == 21 || _userCard == 24) // stop
		{
			m_nTakeCardCount = 1;
			return true;
		}
		else if (_userCard == 26) // +4
		{
			m_nTakeCardCount += 4;
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (_currentCard == 20 || _currentCard == 23)
	{
		if(_userCard == 20 || _userCard == 23) m_bTurn = !m_bTurn;
		else if (_userCard == 25) return false;
		else if (_userCard == 19 || _userCard == 22)
		{
			m_nTakeCardCount = 2;
		}
		else if (_userCard == 26)
		{
			m_nTakeCardCount = 4;
		}
		
		return true;
	}
	else if (_currentCard == 21 || _currentCard == 24) // stop
	{
		if (_userCard == 25) return false;
		return true;
	}
	else if (_currentCard == 25)
	{
		if (_userCard == 25) return false;
		else if (_userCard == 19 || _userCard == 22)
		{
			m_nTakeCardCount = 2;
		}
		else if (_userCard == 20 || _userCard == 23)
		{
			m_bTurn = !m_bTurn;
		}
		else if (_userCard == 26)
		{
			m_nTakeCardCount = 4;
		}
		return true;
	}
	else if (_currentCard == 26)
	{
		if (m_nTakeCardCount == 1)
		{
			if (_userCard == 19 || _userCard == 22)
			{
				m_nTakeCardCount = 2;
			}
			else if (_userCard == 20 || _userCard == 23)
			{
				m_bTurn = !m_bTurn;
			}
			else if (_userCard == 25)
			{
				return false;
			}
			else if (_userCard == 26)
			{
				m_nTakeCardCount = 4;
			}
			return true;
		}
		else if (_userCard == 26)
		{
			m_nTakeCardCount += 4;
			return true;
		}
		else if (_userCard == 21 || _userCard == 24)
		{
			m_nTakeCardCount = 1;
			return true;
		}
		else return false;
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
				if (m_pPlayers[_i].cardCount >= GAME_OVER)
				{
					m_pPlayers[_i].turn = false;
				}
				else
				{
					m_pPlayers[_i].turn = true;
					break;
				}
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
				if (m_pPlayers[_i].cardCount >= GAME_OVER)
				{
					m_pPlayers[_i].turn = false;
				}
				else
				{
					m_pPlayers[_i].turn = true;
					break;
				}
			}
		}
	}
}