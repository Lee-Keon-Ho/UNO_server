#include "app.h"
#pragma comment( lib, "ws2_32.lib")

CApp::CApp()
{
	m_pServer = new CServer(); // 2022-04-23 class : 안좋음.. 그냥 들고 있어도 된다.
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
