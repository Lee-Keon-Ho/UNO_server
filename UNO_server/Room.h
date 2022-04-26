#pragma once

class CRoom
{
public:
	struct stROOM
	{
		int number;
		wchar_t name[64]; // �� �̸�
		int playerCount;
		bool state;
	};

	struct stUser
	{
		int number;
		wchar_t playerName[32]; // ���� �̸�
		int image;
	};
private:
	stROOM m_room;
	stUser* m_pPlayers;

public:
	CRoom();
	CRoom(int num);
	~CRoom();

	void CreateRoom(char* _buffer);
	void OutRoom();
	void InPlayer(char* _buffer);

	int GetNumber() { return m_room.number; }
	int GetPlayerCount() { return m_room.playerCount; }
	wchar_t* GetName() { return m_room.name; }
	stROOM* GetInfo() { return &m_room; }
private:
};