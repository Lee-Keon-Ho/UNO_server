#include "Lobby.h"

CLobby::CLobby()
{

}

CLobby::~CLobby()
{

}

void CLobby::AddUser(CUser* _user)
{
	m_userList.push_back(_user);
}
