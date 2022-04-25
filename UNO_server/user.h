#pragma once
#include "Room.h"
#include <wchar.h>

#define NAME_MAX 32

class CUser
{
public:

private:
	wchar_t m_name[NAME_MAX];
	CRoom* m_pRoom; // 정보만 가지고 있으면된다
public:
	CUser();
	~CUser();

	void SetName(char* _name);
	void SetRoom(CRoom* _room);
	wchar_t* GetName() { return m_name; }
	int GetNumber() { return m_pRoom->GetNumber(); }
	CRoom* GetRoom() { return m_pRoom; }
	void SetRoomInfo(char* _buffer, int _number);
	void DestroyRoom();
};