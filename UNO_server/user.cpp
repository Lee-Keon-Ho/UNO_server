#include "user.h"
#include <stdio.h>

CUser::CUser()
{
}

CUser::~CUser()
{
}

CUser::CUser(SOCKET _socket, SOCKADDR_IN& _addr) 
	: m_socket(_socket), m_addr(_addr)
{
}