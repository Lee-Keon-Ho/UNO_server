#pragma once
#include "fd_set_ex.h" // 2022-04-12 �̸��� ���� �̻��ѵ� ����� �ؾ�����?
#include "userManager.h"
#include "Room.h"
#include "user.h"

class CSelect
{
public:

private:
    SOCKET m_listenSocket;
    fd_set_ex m_fdSocketInfors;

public:
	CSelect();
	~CSelect();
    CSelect(SOCKET _listenSocket);

    void Update();
    void Accept();

    void remove(CSession* _session);
private:
};