#include "Room.h"
#include <memory>

CRoom::CRoom()
{
}

CRoom::CRoom(int _number, wchar_t* _name)
{
	// 수정 필요
	m_room.number = _number;
	memcpy(m_room.name, _name, 32);
	m_room.playerCount = 1;
	m_room.state = true;
}

CRoom::~CRoom()
{
}