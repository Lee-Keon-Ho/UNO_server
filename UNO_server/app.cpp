#include "app.h"
#pragma comment( lib, "ws2_32.lib")

CApp::CApp()
{
	m_pServer = new CServer();
}

CApp::~CApp()
{
}

bool CApp::Initialize(const char* _ip, int _port)
{
	if (!m_pServer->Initialize(_ip, _port)) return false;

	return true;
}

void CApp::Cleanup()
{
	if (m_pServer) { delete m_pServer; m_pServer = nullptr; }
}

void CApp::Update()
{
	m_pServer->Update();
}
