#pragma once
#include "session.h"
#include "Room.h"
#include <wchar.h>

#define NAME_MAX 32
#define START_CARD 7

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
	void Choice(int _cardIndex);
	bool TakeCard(int _card);
	bool Victory();
	void GameOver(bool* _bCard);
	void Boss();

	void SetCard(int _i, int _card) { m_card[_i] = _card; }

	wchar_t* GetName() { return m_MyInfo.name; }
	int GetNumber() { return m_MyInfo.number; }
	stUserInfo* GetGameInfo() { return &m_MyInfo; }
	SOCKET GetSocket() { return m_socket; }
};