#include "User.h"
#include <stdio.h>
#include <string.h>

CUser::CUser()
{
	m_pRoom = new CRoom();
}

CUser::~CUser()
{
	if (m_pRoom) { delete m_pRoom; m_pRoom = nullptr; }
}

void CUser::SetName(char* _name)
{
	memcpy(m_name, _name, sizeof(wchar_t) * NAME_MAX);
}

void CUser::SetRoomInfo(char* _buffer, int _number)
{
	if (m_pRoom == nullptr) m_pRoom = new CRoom();
	memcpy(m_pRoom, _buffer, sizeof(CRoom));
	m_pRoom->SetNumber(_number);
}

void CUser::DestroyRoom(CRoomManager::roomList_t* _roomList)
{
	_roomList->remove(m_pRoom);
	if (m_pRoom) { delete m_pRoom; m_pRoom = nullptr; }
}
