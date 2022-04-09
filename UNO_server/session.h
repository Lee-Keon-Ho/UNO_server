#pragma once
#include "user.h"
#include "Room.h"
#include <WinSock2.h>
#include <WS2tcpip.h>

#define BUFFER_MAX 1000

// CUser : ���Ӽӿ��� player �� ���� ( nickname , �·� ... )

// CSession : ����Ǿ� �ִ� client ���� ( socket - recv, send )

class CSession //Ŀ�ؼ�, ����
{
public:
	enum ePacketType
	{
		CS_PT_NICKNAME = 1,
		CS_PT_CREATEROOM,
		CS_PT_USERLIST,
		CS_PT_ROOMLIST,
		CS_PT_MAX
	};

private:
	SOCKET m_socket;
	SOCKADDR_IN m_addr;
	char m_buffer[BUFFER_MAX];

	CUser* m_pUser; // �ڽ��� ����
	CRoom* m_pRoom; // �ڽ��� ���� ���� ������ �ʿ��Ѱ�?

public:
	CSession();
	~CSession();
	CSession(SOCKET _socket, SOCKADDR_IN& _addr);

	SOCKET GetSock() { return m_socket; }
	int Recv();
	void HandlePacket(int _type);

	void NickName();
	void CreateRoom();
	void UserList();
	void RoomList();

	CUser* GetUser() { return m_pUser; }
private:
};