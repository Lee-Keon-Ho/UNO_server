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

	void PlayerIn(SOCKET _socket);
	void SetName(char* _name);
	bool CreateRoom(char* _name);
	bool RoomIn(char* _playerInfo, SOCKET _socket); // 2022-05-02 ¼öÁ¤
	void RoomOut(SOCKET _socket);
	void SetImage(int _num) { m_image = _num; }
	wchar_t* GetName() { return m_name; }
	int GetRoomNumber() { return m_pRoom->GetNumber(); }
	int GetPlayerCount() { return m_pRoom->GetPlayerCount(); }
	CRoom::stUSER* GetInRoomUserInfo() { return m_pRoom->GetInRoomUserInfo(); }
};