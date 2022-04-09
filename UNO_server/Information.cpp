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

void CInformation::RemoveUser(CSession* _session)
{
	m_userList.remove(_session->GetUser());
}

void CInformation::RemoveRoom(CSession* _session)
{
	//m_roomList.remove();
}
