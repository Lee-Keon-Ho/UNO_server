#include "Information.h"

CInformation* CInformation::pInstance = nullptr;

CInformation* CInformation::GetInstance()
{
	if (pInstance == nullptr) pInstance = new CInformation();
	return pInstance;
}

void CInformation::DeleteInstance()
{
	if (pInstance) { delete pInstance; pInstance = nullptr; }
}

CInformation::CInformation()
{
	
}

CInformation::~CInformation()
{

}

void CInformation::RemoveUser(CUser* _user)
{
	m_userList.remove(_user);
}

void CInformation::RemoveRoom(CRoom* _room)
{
	m_roomList.remove(_room);
}
