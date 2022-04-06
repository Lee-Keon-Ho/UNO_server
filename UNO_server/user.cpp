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

CUser::CUser(char* _recvBuffer)
{
	char* recvtemp = _recvBuffer + 4; // recvSize, type을 제외한 값
	int len = strlen(recvtemp) + 1;
	memcpy(m_pName, recvtemp, len);


	for (int i = 0; i < len; i++)
	{
		printf("%d ", m_pName[i]);
	}
	printf("\n");
}
