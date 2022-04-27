#include "User.h"
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