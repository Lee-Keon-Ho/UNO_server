#pragma once

#define ROOM_NAME_MAX 64
#define USER_NAME_MAX 32

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
	};
private:
	stROOM m_room;
	stUSER m_pPlayers[5];
public:
	CRoom();
	CRoom(int num);
	~CRoom();

	void CreateRoom(char* _name);
	void OutRoom();
	void InPlayer(wchar_t* _name, int _image);
	void InPlayer(char* _playerInfo);

	int GetNumber() { return m_room.number; }
	int GetPlayerCount() { return m_room.playerCount; }
	wchar_t* GetName() { return m_room.name; }
	stROOM* GetInfo() { return &m_room; }
	stUSER* GetInRoomUserInfo() { return m_pPlayers; }
private:
};