#pragma once
#include "Server.h"
#include <stdio.h>

#define MAX 64

class CApp
{
public:
private:
	CServer m_server;
public:
	CApp();
	~CApp();

	bool Initialize(const char* _ip, int _port);
	void Cleanup();

	void Update();

private:
};