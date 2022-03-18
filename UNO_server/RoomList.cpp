#include "RoomList.h"

#define MAX 64

CRoomList::CRoomList()
{
	m_rooms.reserve(MAX);
}

CRoomList::~CRoomList()
{
	
}

void CRoomList::Add(char* _recvBuffer) // _recvBuffer는 어떤 식으로 정보를 담고 있는가?
{
	// room number
	// room name
	// room state
	m_rooms.push_back(new CRoom());
}

void CRoomList::Remove()
{
}
