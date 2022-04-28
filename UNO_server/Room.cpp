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

// clreateRoom -> inplayer
void CRoom::InPlayer(wchar_t* _name, int _image)
{
	// 2022-04-27 수정
	int num = m_room.playerCount - 1;
	m_pPlayers[num].number = num;
	m_pPlayers[num].image = _image;
	memcpy(m_pPlayers[num].playerName, _name, sizeof(wchar_t) * USER_NAME_MAX);
}

void CRoom::InPlayer(char* _playerInfo)
{
	int num = m_room.playerCount;
	m_pPlayers[num].number = num;
	m_room.playerCount += 1;
	// 2022-04-26 수정 : 좋은 버릇은 아닌거 같다.
	m_pPlayers[num].image = *(unsigned short*)_playerInfo;
	memcpy(m_pPlayers[num].playerName, _playerInfo + sizeof(unsigned short), USER_NAME_MAX * sizeof(wchar_t));
}
