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

}