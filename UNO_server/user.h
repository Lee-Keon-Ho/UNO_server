#pragma once
#include "session.h"
#include "Room.h"
#include <wchar.h>

#define NAME_MAX 32

class CUser : public CSession
{
public:
	struct stUserInfo
	{
		wchar_t name[NAME_MAX];
		int image;
		int cardCount;
		int number;
		bool boss;
		bool ready;
		bool turn;
		bool choiceColor;
	};
private:
	stUserInfo m_MyInfo;

	int m_card[USER_CARD_MAX];

	CRoom* m_pRoom;
public:
	CUser();
	CUser(SOCKET _socket, SOCKADDR_IN& _addr);
	~CUser();

	void Setting(int _number, bool _boos, bool _ready, bool _turn);

	void HandlePacket();

	
	// virtual ÇÔ¼öµé
	virtual void OnRecv() override;

	void Start(bool* _bCard);
	void DrawCard(int _cardIndex);
	void DrawChoiceCard(int _cardIndex);
	bool TakeCard(int _card);
	bool Victory();
	void GameOver(bool* _bCard);
	void Boss();
	void Reset();

	void LogIn(char* _info);
	void CreateRoom(char* _info);
	void UserList();
	void RoomList();
	void RoomIn(char* _info);
	void RoomOut();
	void RoomState();
	void Chatting();
	void GameStart();
	void UserDrawCard(char* _cardInfo);
	void ChoiseColor(char* _color);
	void GameReset();
	void RoomInOutChat(const wchar_t* _str);
	void QuickRoomIn();
public:
	void SetCard(int _i, int _card) { m_card[_i] = _card; }

	wchar_t* GetName() { return m_MyInfo.name; }
	int GetNumber() { return m_MyInfo.number; }
	stUserInfo* GetGameInfo() { return &m_MyInfo; }
	SOCKET GetSocket() { return m_socket; }
	int* GetUserCard() { return m_card; }
};