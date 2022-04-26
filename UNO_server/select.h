#pragma once
#include "fd_set_ex.h"
#include "userManager.h"
#include "roomManager.h"

class CSelect
{
private:
    SOCKET m_listenSocket;
    fd_set_ex m_fdSocketInfors;
    CRoomManager* m_roomManager;

    fd_set  fdReads;
    SOCKET  sockTemp;
    int     iRet;
    int     recvSize;
public:
	CSelect();
	~CSelect();
    CSelect(SOCKET _listenSocket);

    void Update();
    void Accept();

    void remove(CSession* _session);
private:
};