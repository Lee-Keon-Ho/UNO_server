#include "select.h"
#include "roomManager.h"
#include "userManager.h"
#include <stdio.h>

#pragma comment( lib, "ws2_32.lib")

CSelect::CSelect()
{
	
}

CSelect::~CSelect()
{

}

CSelect::CSelect(SOCKET _listenSocket) 
	: m_listenSocket(_listenSocket)
{
	FD_ZERO_EX(&m_fdSocketInfors);
	FD_SET_EX(m_listenSocket, nullptr, &m_fdSocketInfors);
	CRoomManager::GetInstance()->Initialize();
}

void CSelect::Update()
{
	while (1)
	{
		fdReads = (fd_set)m_fdSocketInfors;
		iRet = select(0, &fdReads, 0, 0, 0);
		if (iRet == SOCKET_ERROR) return;
		for (unsigned int i = 0; i < m_fdSocketInfors.fd_count; i++)
		{
			sockTemp = m_fdSocketInfors.fd_array[i];
			if (FD_ISSET(sockTemp, &fdReads))
			{
				if (sockTemp == m_listenSocket)
				{
					Accept();
					// : 전체적으로 전부 수정해야한다...
					//m_lobby.AddUser(m_fdSocketInfors.session_array[i]->GetUser());
				}
				else
				{
					recvSize = m_fdSocketInfors.session_array[i]->Recv();
					if (recvSize < 0)
					{
						remove(m_fdSocketInfors.session_array[i]);
						FD_CLR_EX(sockTemp, &m_fdSocketInfors);
					}
				}
			}
		}
	}
}

void CSelect::Accept()
{
	SOCKET sockClient;
	SOCKADDR_IN addrClient;
	int addrSize;
	addrSize = sizeof(addrClient);
	sockClient = accept(m_listenSocket, (SOCKADDR*)&addrClient, &addrSize);
	FD_SET_EX(sockClient, new CUser(sockClient, addrClient),&m_fdSocketInfors);
}

void CSelect::remove(CSession* _session)
{
	//CUserManager::GetInstance()->Remove(_session->GetUser());
};