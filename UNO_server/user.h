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
	void PushBack(char* _chatting);
	void SetName(char* _name);
	void CreateRoom(char* _name);
	void RoomIn(char* _playerInfo, SOCKET _socket); // 2022-05-02 ¼öÁ¤
	void RoomOut();
	void SetImage(int _num) { m_image = _num; }
	wchar_t* GetName() { return m_name; }
	int GetRoomNumber() { return m_pRoom->GetNumber(); }
	int GetPlayerCount() { return m_pRoom->GetPlayerCount(); }
	CRoom::stUSER* GetInRoomUserInfo() { return m_pRoom->GetInRoomUserInfo(); }
	CRoom::chatting_t* GetChatDeque() { return m_pRoom->GetChatDeque(); }
};