#include "roomManager.h"

CRoomManager::CRoomManager() : m_count(0)
{
	m_roomList.reserve(63);
	for (int i = 1; i < 64; i++)
	{
		m_roomList.push_back(new CRoom(i));
	}
}

CRoomManager::~CRoomManager()
{

}

CRoom* CRoomManager::CreateRoom(char* _buffer)
{
	for (int i = 0; i < 63; i++)
	{
		if (m_roomList[i]->GetPlayerCount() == 0)
		{
			m_roomList[i]->CreateRoom(_buffer);
			m_count++;
			return m_roomList[i];
		}
	}
}

void CRoomManager::ResetRoom(int _num)
{
	m_roomList[_num]->ResetInfo();
}

void CRoomManager::OutRoom(int _num)
{
	m_roomList[_num]->OutRoom();
}