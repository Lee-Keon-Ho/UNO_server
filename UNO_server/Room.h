#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>


// gpm 전방선언 - 상호참조를 (즉 서로 include 를 하는 경우) 피할 수 있는 방법
// class CUser;
// 반드시 cpp에 #include " " 가 있어야 함..


#define ROOM_NAME_MAX 64
#define USER_NAME_MAX 32
#define PLAYER_MAX 5
#define CARD_ALL 110
#define USER_CARD_MAX 12
#define COLOR_MAX 5
#define CARD_MAX 27
#define USER_CARD_MAX 20

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
		int card[USER_CARD_MAX];
		int cardCount;
		bool turn;
	};

	struct stCARD
	{
		char number;
		char color;
	};

private:
	//CUser*  m_Users[PLAYER_MAX]; 상호 참조를 해결하면 가능하다.

	stROOM m_room;
	stUSER m_pPlayers[PLAYER_MAX];
	stCARD m_Card[CARD_ALL]; // 속성이 있는 카드
	bool m_bCard[CARD_ALL]; 
	int m_currentCard; // 현재 카드의 index
	bool m_bTurn; // 방향
	int m_nTakeCardCount;
	int m_nColor;
public:
	CRoom();
	CRoom(int num);
	~CRoom();

	//int GetUserNum(void); { return m_Users[0]->GetPlayerCount(); }

	void CreateRoom(char* _name);
	bool RoomOut(SOCKET _socket);
	void PlayerIn(wchar_t* _name, int _image, SOCKET _socket);
	bool PlayerIn(char* _playerInfo, SOCKET _socket);
	void Ready(SOCKET _socket);
	void Start();
	void DrawCard(SOCKET _socket, int _card, int _userCardindex);
	void TakeCard(SOCKET _socket);

	int GetNumber() { return m_room.number; }
	int GetPlayerCount() { return m_room.playerCount; }
	int GetCurrentCard() { return m_currentCard; }
	wchar_t* GetName() { return m_room.name; }
	stROOM* GetInfo() { return &m_room; }
	stUSER* GetInRoomUserInfo() { return m_pPlayers; }

	bool Compare(int _x1, int _x2);
	void NextTurn(bool _turn, int _i);
private:
};