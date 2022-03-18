#pragma once
#include "list.h"
#include "Room.h"
#include <vector>

class CRoomList : public CList
{
public:
	typedef std::vector<CRoom*> RoomList;
private:
	RoomList m_rooms;
public:
	CRoomList();
	~CRoomList();
	
	void Add(char* _recvBuffer);
	void Remove();
private:
};