#pragma once
#include <wchar.h>
#include "roomManager.h"

#define NAME_MAX 32

class CUser
{
public:

private:
	wchar_t m_name[NAME_MAX];
	CRoom* m_pRoom;
public:
	CUser();
	~CUser();

	void SetName(char* _name);
	void SetRoom(CRoom* _room);
	wchar_t* GetName() { return m_name; }
	CRoom* GetRoom() { return m_pRoom; }
	void SetRoomInfo(char* _buffer, int _number);
	void DestroyRoom(CRoomManager::roomList_t* _roomList);
};