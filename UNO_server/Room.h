#pragma once

// ¹æ ÇÏ³ª
class CRoom
{
public:
	struct stROOM
	{
		int number;
		wchar_t name[64];
		int playerCount;
		bool state;
	};
private:
	stROOM m_room;

public:
	CRoom();
	~CRoom();

	void SetNumber(int _num);
	wchar_t* GetName() { return m_room.name; }
	stROOM* GetInfo() { return &m_room; }
private:
};