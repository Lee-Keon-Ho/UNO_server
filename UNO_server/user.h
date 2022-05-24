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
	bool RoomIn(char* _playerInfo, SOCKET _socket);
	void RoomOut(SOCKET _socket);
	void Ready(SOCKET _socket);
	void Start();
	void DrawCard(SOCKET _socket, int _card, int _index);
	void TakeCard(SOCKET _socket);
	void ChoiceColor(SOCKET _socket, int _color);
	void Victory();

	//void HandlePacket(int _type);

	// virtual ÇÔ¼öµé
	//void OnRecv(char* _buffer) override ;


	void SetImage(int _num) { m_image = _num; }

	wchar_t* GetName() { return m_name; }
	int GetRoomNumber() { return m_pRoom->GetNumber(); }
	int GetPlayerCount() { return m_pRoom->GetPlayerCount(); }
	int GetCurrentCard() { return m_pRoom->GetCurrentCard(); }
	CRoom::stROOM* GetRoominfo() { return m_pRoom->GetInfo(); }
	CRoom::stUSER* GetInRoomUserInfo() { return m_pRoom->GetInRoomUserInfo(); }


};