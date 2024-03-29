#include "userManager.h"

CUserManager* CUserManager::pInstance = nullptr;

CUserManager* CUserManager::GetInstance()
{
	if (pInstance == nullptr) pInstance = new CUserManager();
	return pInstance;
}

void CUserManager::DeleteInstance()
{
	if (pInstance) { delete pInstance; pInstance = nullptr; }
}

CUserManager::CUserManager()
{

}

CUserManager::~CUserManager()
{

}

void CUserManager::AddUser(CUser* _user)
{
	m_userList.push_back(_user);
}

void CUserManager::Remove(CUser* _user)
{
	m_userList.remove(_user);
}