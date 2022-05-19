#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>


// gpm ���漱�� - ��ȣ������ (�� ���� include �� �ϴ� ���) ���� �� �ִ� ���
// class CUser;
// �ݵ�� cpp�� #include " " �� �־�� ��..


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
		wchar_t name[ROOM_NAME_MAX]; // �� �̸�
		int playerCount;
		bool state;
	};

	struct stUSER
	{
		int number;
		wchar_t playerName[USER_NAME_MAX]; // ���� �̸�
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
	//CUser*  m_Users[PLAYER_MAX]; ��ȣ ������ �ذ��ϸ� �����ϴ�.

	stROOM m_room;
	stUSER m_pPlayers[PLAYER_MAX];
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