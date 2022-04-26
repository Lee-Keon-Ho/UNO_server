#pragma once
#include "Room.h"
#include <wchar.h>

#define NAME_MAX 32

class CUser
{
private:
	wchar_t m_name[NAME_MAX];
	int m_image;
	CRoom* m_pRoom;
public:
	CUser();
	~CUser();

	void SetName(char* _name);
	void SetRoom(CRoom* _room);
	void SetImage(int _num) { m_image = _num; }
	wchar_t* GetName() { return m_name; }
	int GetRoomNumber() { return m_pRoom->GetNumber(); }
	CRoom* GetRoom() { return m_pRoom; }
	void SetRoomInfo(char* _buffer, int _number);
	void DestroyRoom();
};