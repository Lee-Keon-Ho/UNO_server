#pragma once

// �� �ϳ�
class CRoom
{
public:
	struct Room
	{
		int number;
		char name[64];
		int playerCount;
		bool state;
	};
private:
	// ���̸� (�������̸����� ����� ����)
	// �� ��ȣ
	// ���� ���� ��Ȳ
	Room m_room;
public:
	CRoom();
	CRoom(char* _name);
	~CRoom();
private:
};