#include "app.h"
#pragma comment( lib, "ws2_32.lib")

CApp::CApp()
{
	
}

CApp::~CApp()
{
}

bool CApp::Initialize(const char* _ip, int _port)
{
	if (!m_server.Initialize(_ip, _port)) return false;

	return true;
}

void CApp::Cleanup()
{
	m_server.Cleanup();
}

void CApp::Update()
{
	m_server.Update();
}