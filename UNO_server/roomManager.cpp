#include "roomManager.h"

CRoomManager* CRoomManager::pInstance = nullptr;

CRoomManager* CRoomManager::GetInstance()
{
	if (pInstance == nullptr) pInstance = new CRoomManager();
	return pInstance;
}

void CRoomManager::DeleteInstance()
{
	if (pInstance) { delete pInstance; pInstance = nullptr; }
}

CRoomManager::CRoomManager()
{
	
}

CRoomManager::~CRoomManager()
{
	Cleanup();
}

bool CRoomManager::Initialize()
{
	m_roomList.reserve(63);
	for (int i = 1; i < 64; i++)
	{
		m_roomList.push_back(new CRoom(i));
	}

	m_count = 0;
	return true;
}

void CRoomManager::Cleanup()
{
	std::vector<CRoom*>::iterator iter = m_roomList.begin();
	std::vector<CRoom*>::iterator endIter = m_roomList.end();
	for (; iter != endIter; iter++)
	{
		if (*iter) { delete (*iter); (*iter) = nullptr; }
	}
}


CRoom* CRoomManager::CreateRoom(char* _name)
{
	for (int i = 0; i < 63; i++)
	{
		if (m_roomList[i]->GetPlayerCount() == 0)
		{
			m_roomList[i]->CreateRoom(_name);
			m_count++;
			return m_roomList[i];
		}
	}
	return nullptr;
}

CRoom* CRoomManager::RoomIn(int _roomNumber)
{
	int playercount = m_roomList[_roomNumber]->GetPlayerCount();
	if (playercount != 0 && playercount < PLAYER_MAX)
	{
		return m_roomList[_roomNumber];
	}
	return nullptr;	
}

void CRoomManager::RoomOut()
{
	m_count--;
	if (m_count < 0) m_count = 0;
}