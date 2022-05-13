#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>

#define ROOM_NAME_MAX 64
#define USER_NAME_MAX 32
#define PLAYER_MAX 5
#define CARD_MAX 110
#define USER_CARD_MAX 12

class CRoom
{
public:
	struct stROOM
	{
		int number;
		wchar_t name[ROOM_NAME_MAX]; // 방 이름
		int playerCount;
		bool state;
	};

	struct stUSER
	{
		int number;
		wchar_t playerName[USER_NAME_MAX]; // 유저 이름
		int image;
		SOCKET socket;
		bool boss;
		bool ready;
		int card[12];
		int cardCount;
	};

private:
	stROOM m_room;
	stUSER m_pPlayers[PLAYER_MAX];
	bool m_card[CARD_MAX];
	int m_currentCard;
public:
	CRoom();
	CRoom(int num);
	~CRoom();

	void CreateRoom(char* _name);
	bool RoomOut(SOCKET _socket);
	void PlayerIn(wchar_t* _name, int _image, SOCKET _socket);
	bool PlayerIn(char* _playerInfo, SOCKET _socket);
	void Ready(SOCKET _socket);
	void Start();

	int GetNumber() { return m_room.number; }
	int GetPlayerCount() { return m_room.playerCount; }
	int GetCurrentCard() { return m_currentCard; }
	wchar_t* GetName() { return m_room.name; }
	stROOM* GetInfo() { return &m_room; }
	stUSER* GetInRoomUserInfo() { return m_pPlayers; }
private:
};