#include "User.h"
#include "roomManager.h"
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

void CUser::SetRoom(CRoom* _room)
{
	m_pRoom = _room;
}

void CUser::SetRoomInfo(char* _buffer, int _number)
{
	if (m_pRoom == nullptr) m_pRoom = new CRoom();
	m_pRoom->SetInfo(_buffer, _number);	
}

void CUser::DestroyRoom()
{
	CRoomManager::GetInstance()->GetRoomList()->remove(m_pRoom);
	if (m_pRoom) { delete m_pRoom; m_pRoom = nullptr; }
}
