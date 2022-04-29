#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <deque>

#define ROOM_NAME_MAX 64
#define USER_NAME_MAX 32
#define PLAYER_MAX 5

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
	};

	typedef std::deque<char*> chatting_t;

private:
	stROOM m_room;
	stUSER m_pPlayers[PLAYER_MAX];
	chatting_t m_chatting;
public:
	CRoom();
	CRoom(int num);
	~CRoom();

	void CreateRoom(char* _name);
	void OutRoom();
	void InPlayer(wchar_t* _name, int _image, SOCKET _socket);
	void InPlayer(char* _playerInfo, SOCKET _socket);
	void PushBack(char* _chatting);

	int GetNumber() { return m_room.number; }
	int GetPlayerCount() { return m_room.playerCount; }
	wchar_t* GetName() { return m_room.name; }
	stROOM* GetInfo() { return &m_room; }
	stUSER* GetInRoomUserInfo() { return m_pPlayers; }
	chatting_t* GetChatDeque() { return &m_chatting; }
private:
};