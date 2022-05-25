#pragma once
#include "session.h"
#include "Room.h"
#include <wchar.h>


#define NAME_MAX 32

class CUser : public CSession
{
private:
	wchar_t m_name[NAME_MAX];
	int m_image;
	CRoom* m_pRoom;

public:
	CUser();
	CUser(SOCKET _socket, SOCKADDR_IN& _addr);
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

	void HandlePacket();

	// virtual ÇÔ¼öµé
	virtual void OnRecv() override;

	void SetImage(int _num) { m_image = _num; }

	wchar_t* GetName() { return m_name; }
	int GetRoomNumber() { return m_pRoom->GetNumber(); }
	int GetCurrentCard() { return m_pRoom->GetCurrentCard(); }
};