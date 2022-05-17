#include "User.h"
#include "roomManager.h"
#include <stdio.h>
#include <string.h>

CUser::CUser()
{
	
}

CUser::~CUser()
{
	
}

void CUser::PlayerIn(SOCKET _socket)
{ 
	m_pRoom->PlayerIn(m_name, m_image, _socket);
}

void CUser::SetName(char* _name)
{
	memcpy(m_name, _name, sizeof(wchar_t) * NAME_MAX);
}

bool CUser::CreateRoom(char* _name)
{
	m_pRoom = CRoomManager::GetInstance()->CreateRoom(_name);
	if (m_pRoom == nullptr)
	{
		return false;
	}
	return true;
}

//2022-05-02 수정
bool CUser::RoomIn(char* _playerInfo, SOCKET _socket)
{
	m_pRoom = CRoomManager::GetInstance()->RoomIn(_playerInfo, _socket);
	if (m_pRoom == nullptr)
	{
		return false;
	}
	return true;
}

void CUser::RoomOut(SOCKET _socket)
{
	// 2022-05-17 수정하자.
	if (!m_pRoom->RoomOut(_socket)) CRoomManager::GetInstance()->RoomOut();
}

void CUser::Ready(SOCKET _socket)
{
	m_pRoom->Ready(_socket);
}

void CUser::Start()
{
	m_pRoom->Start();
}

void CUser::DrawCard(SOCKET _socket, int _card, int _index)
{
	m_pRoom->DrawCard(_socket, _card, _index);
}
