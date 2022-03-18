#include "Room.h"

#define MAX 255

CRoom::CRoom()
{
	m_roomName = new char[MAX];
	m_roomNum = new char[MAX];
	m_state = new char[MAX];
}

CRoom::~CRoom()
{
	if (m_state) { delete m_state; m_state = nullptr; }
	if (m_roomNum) { delete m_roomNum; m_roomNum = nullptr; }
	if (m_roomName) { delete m_roomName; m_roomName = nullptr; }
}