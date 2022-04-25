#include "Room.h"
#include <memory>

CRoom::CRoom()
{
}

CRoom::CRoom(int num)
{
	m_room.number = num;
	m_room.playerCount = 0;
	m_room.state = true;
}

CRoom::~CRoom()
{
}

void CRoom::CreateRoom(char* _buffer)
{
	memcpy(m_room.name, _buffer + 4, 128);
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
	}
}

void CRoom::ResetInfo()
{
	memset(m_room.name, 0, 64);
	m_room.playerCount = 0;
	m_room.state = true;
}

void CRoom::SetInfo(char* _buffer)
{
	memcpy(&m_room, _buffer, sizeof(CRoom::stROOM));
}