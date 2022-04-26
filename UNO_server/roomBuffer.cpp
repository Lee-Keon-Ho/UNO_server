#include "roomBuffer.h"

CRoomBuffer* CRoomBuffer::pInstance = nullptr;

CRoomBuffer* CRoomBuffer::GetInstance()
{
	if (pInstance == nullptr) pInstance = new CRoomBuffer;
	return pInstance;
}

void CRoomBuffer::DeleteInstance()
{
	if (pInstance) { delete pInstance; pInstance = nullptr; }
}

CRoomBuffer::CRoomBuffer()
{

}

CRoomBuffer::~CRoomBuffer()
{

}

CRoom* CRoomBuffer::CreateRoom(char* _buffer)
{
	return m_pRoomManager->CreateRoom(_buffer);
}

void CRoomBuffer::DestroyRoom()
{
}

CRoom* CRoomBuffer::InRoom(char* _buffer)
{
	return m_pRoomManager->InRoom(_buffer);
}

void CRoomBuffer::RoomState()
{
}

void CRoomBuffer::OutRoom(int _num)
{
	m_pRoomManager->OutRoom(_num);
}