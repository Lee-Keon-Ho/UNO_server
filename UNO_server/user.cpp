#include "User.h"
#include <string.h>

#define MAX 64

CUser::CUser()
{
}

CUser::~CUser()
{
}

CUser::CUser(char* _recvBuffer)
{
	char* recvtemp = _recvBuffer + 4; // recvSize, type�� ������ ��
	m_pName = new char[MAX];
	memcpy(m_pName, recvtemp, strlen(recvtemp) + 1);
}
