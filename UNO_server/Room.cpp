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

	m_pPlayers = new stUser[PLAYER_MAX];
}

CRoom::~CRoom()
{
	if (m_pPlayers) { delete[] m_pPlayers; m_pPlayers = nullptr; }
}

void CRoom::CreateRoom(char* _buffer)
{
	// 2022-04-26 ¼öÁ¤ : test
	memcpy(m_room.name, _buffer + 6, 128);
	m_room.playerCount = 1;
}

void CRoom::OutRoom()
{
	if (m_room.playerCount > 0)
	{
		m_room.playerCount -= 1;
	}
	if (m_room.playerCount <= 0)
	{
		memset(m_room.name, 0, 64);
		m_room.playerCount = 0;
		m_room.state = true;
	}
}

void CRoom::InPlayer(char* _buffer)
{
	m_room.playerCount += 1;
	m_pPlayers[m_room.playerCount].image = *(unsigned int*)_buffer;
	memcpy(m_pPlayers[m_room.playerCount].playerName, _buffer, 32);
}
