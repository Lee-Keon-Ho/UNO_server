#include "Room.h"
#include <memory>

CRoom::CRoom()
{
}

CRoom::~CRoom()
{
}

void CRoom::SetInfo(char* _buffer, int _num)
{
	memcpy(&m_room, _buffer, sizeof(CRoom::stROOM));
	m_room.number = _num;
}
