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

	CUser* m_pUser;  // 2022-05-21 ��ӱ����� �����϶��!!

public:
	CSession();
	~CSession();
	CSession(SOCKET _socket, SOCKADDR_IN& _addr);

	int Recv();
	void HandlePacket(int _type);

	//virtual void OnRecv(char* m_buffer);


	void Login();
	void CreateRoom();
	void UserList();
	void RoomList();
	void RoomIn();
	void RoomOut();
	void RoomState();
	void Chatting();
	void Ready();
	void Start();
	void DrawCard();
	void TakeCard();
	void ChoiceColor();

	SOCKET GetSock() { return m_socket; }
	CUser* GetUser() { return m_pUser; }
private:
};