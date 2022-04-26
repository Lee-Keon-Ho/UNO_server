#pragma once
#include "roomManager.h"

// 2022-04-25 ���� : roomManager �̸��� rooms�� �ٲٰ� 
// roombuffer�� roomManager�� �ٲٴ°� �´°� ����.
class CRoomBuffer
{
private:
	static CRoomBuffer* pInstance;
	CRoomBuffer();
	~CRoomBuffer();

public:
	CRoomManager* m_pRoomManager; 

public:
	CRoom* CreateRoom(char* _buffer);
	void DestroyRoom();
	CRoom* InRoom(char* _buffer);
	void RoomState();
	void OutRoom(int _num);

	void SetRoomManager(CRoomManager* _roomManager) { m_pRoomManager = _roomManager; }
	CRoomManager* GetRoomManager() { return m_pRoomManager; }
public:
	static CRoomBuffer* GetInstance();
	static void DeleteInstance();
};