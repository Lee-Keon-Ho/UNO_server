#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>

class CUser;

// gpm ���漱�� - ��ȣ������ (�� ���� include �� �ϴ� ���) ���� �� �ִ� ���
// class CUser;
// �ݵ�� cpp�� #include " " �� �־�� ��..


#define ROOM_NAME_MAX 64
#define USER_NAME_MAX 32
#define PLAYER_MAX 5
#define CARD_ALL 108
#define COLOR_MAX 4
#define CARD_MAX 27
#define USER_CARD_MAX 20
#define GAME_OVER 19

class CRoom
{
public:
	struct stROOM
	{
		int number;
		wchar_t name[ROOM_NAME_MAX]; // �� �̸�
		int playerCount;
		bool state;
		bool victory;
	};

	struct stCARD
	{
		char number;
		char color;
	};

private:
	CUser*  m_Users[PLAYER_MAX]; //

	int m_nUserCount;

	stROOM m_room;
	stCARD m_Card[CARD_ALL]; // �Ӽ��� �ִ� ī��
	bool m_bCard[CARD_ALL]; 
	int m_currentCard; // ���� ī���� index
	bool m_bTurn; // ����
	int m_nTakeCardCount;
	int m_nColor;
public:
	CRoom();
	CRoom(int num);
	~CRoom();

	//int GetUserNum(void); { return m_Users[0]->GetPlayerCount(); }

	void CreateRoom(char* _name);
	bool RoomOut(SOCKET _socket);
	void PlayerIn(CUser* _user);
	void Start();
	void DrawCard(SOCKET _socket, int _card, int _userCardindex);
	void TakeCard(SOCKET _socket);
	void ChoiceColor(SOCKET _socket, int _color);
	//void Victory();

	int GetNumber() { return m_room.number; }
	int GetPlayerCount() { return m_room.playerCount; }
	int GetCurrentCard() { return m_currentCard; }
	wchar_t* GetName() { return m_room.name; }
	stROOM* GetInfo() { return &m_room; }
	CUser** GetUser() { return m_Users; }
	bool NumCompare(int _userCard, int _currentCard);
	bool ColorCompare(int _userCard, int _currentCard);
	void NextTurn(bool _turn, int _i);
private:
};