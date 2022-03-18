#include "session.h"
#include <stdio.h>

CSession::CSession()
{
}

CSession::~CSession()
{
}

CSession::CSession(SOCKET _socket, SOCKADDR_IN& _addr)
	: m_socket(_socket), m_addr(_addr)
{
}