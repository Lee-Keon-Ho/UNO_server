#include "Room.h"
#include <memory>

#define PLAYER_MAX 5

CRoom::CRoom()
{
}

CRoom::CRoom(int num)
{
	m_room.number = num;
	m_room.playerCount = 0;
	m_room.state = true;

	memset(m_pPlayers, 0, sizeof(stUSER) * PLAYER_MAX);
}

CRoom::~CRoom()
{
	
}

void CRoom::CreateRoom(char* _name)
{
	// 2022-04-26 수정 : test
	memcpy(m_room.name, _name, ROOM_NAME_MAX * sizeof(wchar_t));
	m_room.playerCount = 1;
}

bool CRoom::RoomOut(SOCKET _socket)
{
	bool bRoom = true;
	if (m_room.playerCount > 0)
	{
		m_room.playerCount -= 1;

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
					for (int j = i; j < PLAYER_MAX; j++)
					{
						if (j + 1 < 5)
						{
							m_pPlayers[j] = m_pPlayers[j + 1];
						}	
					}
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
	m_pPlayers[num].number = num;
	m_pPlayers[num].image = _image;
	memcpy(m_pPlayers[num].playerName, _name, sizeof(wchar_t) * USER_NAME_MAX);
	m_pPlayers[num].socket = _socket;
}

bool CRoom::PlayerIn(char* _playerInfo, SOCKET _socket)
{
	int num = m_room.playerCount;
	if (num > 4)
	{
		return false;
	}
	m_pPlayers[num].number = num;
	m_room.playerCount += 1;
	m_pPlayers[num].image = *(unsigned short*)_playerInfo;
	memcpy(m_pPlayers[num].playerName, _playerInfo + sizeof(unsigned short), USER_NAME_MAX * sizeof(wchar_t));
	m_pPlayers[num].socket = _socket;
	return true;
}

void CRoom::PushBack(char* _chatting)
{
	// 2022-04-29 수정 : test
	char* pChat = new char[64];
	memcpy(pChat, _chatting, 64);
	m_chatting.push_back(pChat);
}

void CRoom::ReSetChat()
{
	chatting_t::iterator iter = m_chatting.begin();
	chatting_t::iterator endIter = m_chatting.end();

	for (; iter != endIter; iter++)
	{
		delete[] *iter;
	}

	m_chatting.clear();
}
