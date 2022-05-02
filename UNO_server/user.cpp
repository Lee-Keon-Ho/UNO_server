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

void CUser::PushBack(char* _chatting)
{
	m_pRoom->PushBack(_chatting);
}

void CUser::SetName(char* _name)
{
	memcpy(m_name, _name, sizeof(wchar_t) * NAME_MAX);
}

void CUser::CreateRoom(char* _name)
{
	m_pRoom = CRoomManager::GetInstance()->CreateRoom(_name);
}

//2022-05-02 ¼öÁ¤
void CUser::RoomIn(char* _playerInfo, SOCKET _socket)
{
	m_pRoom = CRoomManager::GetInstance()->RoomIn(_playerInfo, _socket);
}

void CUser::RoomOut()
{
	if (!m_pRoom->RoomOut()) CRoomManager::GetInstance()->RoomOut();
	m_pRoom->ReSetChat();
}
