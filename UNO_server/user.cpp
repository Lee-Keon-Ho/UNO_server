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
	
}

void CUser::DestroyRoom()
{

}
