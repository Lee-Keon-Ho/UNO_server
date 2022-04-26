#pragma once
#include "roomManager.h"

// 2022-04-25 수정 : roomManager 이름을 rooms로 바꾸고 
// roombuffer를 roomManager로 바꾸는게 맞는거 같다.
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