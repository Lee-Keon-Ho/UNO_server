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

	m_pPlayers = new stUSER[PLAYER_MAX];
}

CRoom::~CRoom()
{
	if (m_pPlayers) { delete[] m_pPlayers; m_pPlayers = nullptr; }
}

void CRoom::CreateRoom(char* _buffer)
{
	// 2022-04-26 수정 : test
	memcpy(m_room.name, _buffer, ROOM_NAME_MAX * sizeof(wchar_t));
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
		memset(m_room.name, 0, ROOM_NAME_MAX * sizeof(wchar_t));
		m_room.playerCount = 0;
		m_room.state = true;
	}
}

void CRoom::InPlayer(char* _buffer)
{
	m_room.playerCount += 1;
	// 2022-04-26 수정 : 좋은 버릇은 아닌거 같다.
	m_pPlayers[m_room.playerCount].image = *(unsigned int*)_buffer;
	memcpy(m_pPlayers[m_room.playerCount].playerName, _buffer, USER_NAME_MAX * sizeof(wchar_t));
}
