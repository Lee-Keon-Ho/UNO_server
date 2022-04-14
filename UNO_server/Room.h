#pragma once

// ¹æ ÇÏ³ª
class CRoom
{
public:
	struct stROOM
	{
		int number;
		wchar_t name[32];
		int playerCount;
		bool state;
	};
private:
	stROOM m_room;

public:
	CRoom();
	CRoom(int _number, wchar_t* _name);
	~CRoom();
private:
};