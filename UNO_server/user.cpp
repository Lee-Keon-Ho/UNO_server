#include "User.h"
#include <stdio.h>
#include <string.h>

#define MAX 64

CUser::CUser()
{
}

CUser::~CUser()
{
}

CUser::CUser(CUser* _recvBuffer)
{
	memcpy(this, _recvBuffer, sizeof(CUser));
	//WCHAR* recvtemp = _recvBuffer + 4; // recvSize, type을 제외한 값
	//int len = sizeof(WCHAR) * wcslen(recvtemp) + 1;
	//memcpy(m_name, recvtemp, len);
	
	wprintf(L"%s \n", m_name);
}
