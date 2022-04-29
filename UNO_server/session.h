#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>
#include "user.h"

#define BUFFER_MAX 1000

// CUser : ���Ӽӿ��� player �� ���� ( nickname , �·� ... )

// CSession : ����Ǿ� �ִ� client ���� ( socket - recv, send )

class CSession //Ŀ�ؼ�, ����
{
private:
	SOCKET m_socket;
	SOCKADDR_IN m_addr;
	char m_buffer[BUFFER_MAX];

	CUser* m_pUser; 

public:
	CSession();
	~CSession();
	CSession(SOCKET _socket, SOCKADDR_IN& _addr);

	int Recv();
	void HandlePacket(int _type);

	void Login();
	void CreateRoom();
	void UserList();
	void RoomList();
	void InRoom();
	void OutRoom();
	void RoomState();
	void Chatting();

	SOCKET GetSock() { return m_socket; }
	CUser* GetUser() { return m_pUser; }
private:
};