#include "Room.h"
#include "user.h"
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

	for (int color = 0; color < COLOR_MAX; color++)
	{
		for (int card = 0; card < CARD_MAX; card++)
		{
			m_Card[(color * CARD_MAX) + card].number = card;
			m_Card[(color * CARD_MAX) + card].color = color;
		}
	}

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		m_Users[i] = nullptr;
	}
	memset(m_bCard, 1, CARD_MAX);
}

CRoom::~CRoom()
{
	
}

void CRoom::CreateRoom(char* _name)
{
	memcpy(m_room.name, _name, ROOM_NAME_MAX * sizeof(wchar_t));
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
				m_Users[i] = nullptr;
			}
			bRoom = false;
		}
		else
		{
			for (int i = 0; i < PLAYER_MAX; i++)
			{
				if (m_Users[i]->GetSocket() == _socket)
				{
					m_Users[i] = nullptr;
					break;
				}
			}

			for (int i = 0; i < PLAYER_MAX; i++)
			{
				if (m_Users[i] != nullptr)
				{
					m_Users[i]->Boss();
					break;
				}
			}
			bRoom = true;
		}
	}
	return bRoom;
}

void CRoom::PlayerIn(CUser* _user)
{
	int playerCount = m_room.playerCount;

	if (m_room.playerCount == 0)
	{
		m_Users[playerCount] = _user;
		m_Users[playerCount]->Setting(playerCount + 1, true, true, true);
	}
	else
	{
		for (int i = 0; i < PLAYER_MAX; i++)
		{
			if (m_Users[i] == nullptr)
			{
				m_Users[i] = _user;
				m_Users[i]->Setting(i + 1, false, false, false);
				break;
			}
		}
	}
	m_room.playerCount++;
}

void CRoom::Start()
{
	int nCard;
	for (int player = 0; player < m_room.playerCount; player++)
	{
		if (m_Users[player]->GetNumber() != 0)
		{
			m_Users[player]->Start(m_bCard);
		}
	}

	while (true)
	{
		nCard = rand() % CARD_ALL;
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

bool CRoom::DrawCard(CUser* _pUser, int _card, int _userCardindex)
{
	stCARD userCard = m_Card[_card];
	stCARD currentCard = m_Card[m_currentCard];
	bool bTurn = false;

	if (userCard.color == m_nColor)
	{
		if (ColorCompare(userCard.number, currentCard.number))
		{
			m_bCard[_card] = true;
			m_currentCard = _card;
			bTurn = true;
		}
		else if (userCard.number == 25)
		{
			_pUser->DrawChoiceCard(_userCardindex);
			m_bCard[_card] = true;
			bTurn = false;
		}
		else bTurn = false;
	}
	else if (NumCompare(userCard.number, currentCard.number))
	{
		if (userCard.number == 25)
		{
			bTurn = false;
		}
		else
		{
			_pUser->GetGameInfo()->turn = false;
			bTurn = true;
		}
		m_nColor = userCard.color;
		m_currentCard = _card;
	}
	else bTurn = false;

	if (bTurn)
	{
		_pUser->DrawCard(_userCardindex);
		if (_pUser->Victory())
		{
			m_room.victory = true;
		}
		m_bCard[_card] = true;
		for (int i = 0; i < PLAYER_MAX; i++)
		{
			if (m_Users[i] == _pUser)
			{
				NextTurn(m_bTurn, i);
				break;
			}
		}
		return true;
	}
	else
	{
		if (_pUser->Victory()) m_room.victory = true;
		return false;
	}
}

void CRoom::TakeCard(CUser* _pUser)
{
	int nCard;
	for (int index = 0; index < PLAYER_MAX; index++)
	{
		if (m_Users[index] == _pUser)
		{	
			for (int i = 0; i < m_nTakeCardCount; i++)
			{
				while (true)
				{
					nCard = rand() % CARD_ALL;
					if (m_bCard[nCard])
					{
						if (!m_Users[index]->TakeCard(nCard))
						{
							m_Users[index]->GameOver(m_bCard);
						}
						break;
					}
				}
			}
			m_Users[index]->GetGameInfo()->turn = false;
			m_nTakeCardCount = 1;
			NextTurn(m_bTurn, index);
		}
	}
}

void CRoom::ChoiceColor(CUser* _pUser, int _color)
{
	m_currentCard = 25 + (_color * 27);
	m_nColor = _color;
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (m_Users[i] == _pUser)
		{
			m_Users[i]->GetGameInfo()->choiceColor = false;
			m_Users[i]->GetGameInfo()->turn = false;
			NextTurn(m_bTurn, i);
		}
	}
}

void CRoom::Reset()
{
	m_room.state = true;
	m_room.victory = false;
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (m_Users[i] != nullptr)
		{
			m_Users[i]->Reset();
		}
	}
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
				return true;
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
			if (m_Users[_i] != nullptr)
			{
				if (m_Users[_i]->GetGameInfo()->cardCount >= GAME_OVER)
				{
					m_Users[_i]->GetGameInfo()->turn = false;
					break;
				}
				else
				{
					m_Users[_i]->GetGameInfo()->turn = true;
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
			if (m_Users[_i] != nullptr)
			{
				if (m_Users[_i]->GetGameInfo()->cardCount >= GAME_OVER)
				{
					m_Users[_i]->GetGameInfo()->turn = false;
					break;
				}
				else
				{
					m_Users[_i]->GetGameInfo()->turn = true;
					break;
				}
			}
		}
	}
}

int* CRoom::GetUserCard(int _i)
{
	return m_Users[_i]->GetUserCard();
}

SOCKET CRoom::GetUserSocket(int _i)
{
	return m_Users[_i]->GetSocket();
}
