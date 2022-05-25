#include "session.h"
#include "userManager.h"
#include "roomManager.h"
#include <stdio.h>

#define CHATBUFFER 64

CSession::CSession()
{
	// 2022-05-25 수정
	//m_pUser = new CUser();
}

CSession::~CSession()
{
	//if (m_pUser) { delete m_pUser; m_pUser = nullptr; }
	closesocket(m_socket);
}

CSession::CSession(SOCKET _socket, SOCKADDR_IN& _addr)
	: m_socket(_socket), m_addr(_addr)
{
	// 2022-05-25 수정
	//m_pUser = new CUser();
}

int CSession::Recv()
{
	int recvedSize = 0;
	int recvLen = recv(m_socket, m_buffer + recvedSize, BUFFER_MAX - recvedSize, 0);
	if (recvLen < 1)  return recvLen;

	recvedSize += recvLen;

	if (recvedSize < 2)  return recvLen;

	unsigned short packetSize = *(unsigned short*)m_buffer;
	unsigned short type = *(unsigned short*)(m_buffer + 2);

	while (recvedSize >= packetSize)
	{
		// 2022-05-25 수정
		OnRecv();
		//HandlePacket(type);

		recvedSize -= packetSize;
		if (recvedSize > 0)
		{
			memcpy(m_buffer, m_buffer + packetSize, recvedSize);

			if (recvedSize > 1) 
			{
				packetSize = *(unsigned short*)m_buffer;
			}
			if (recvedSize > 3)
			{
				type = *(unsigned short*)(m_buffer + 2);
			}
		}
	}

	return recvLen;
}

void CSession::OnRecv()
{
}
//void CSession::Chatting()
//{
//	char buffer[CHATBUFFER];
//	memset(buffer, 0, CHATBUFFER);
//	char* tempBuffer = buffer;
//
//	unsigned short packetSize = *(unsigned short*)m_buffer;
//
//	int size = wcslen(m_pUser->GetName()) * sizeof(wchar_t);
//	memcpy(tempBuffer, m_pUser->GetName(), size);
//	tempBuffer += size;
//	memcpy(tempBuffer, L" : ", wcslen(L" : ") * sizeof(wchar_t));
//	tempBuffer += wcslen(L" : ") * sizeof(wchar_t);
//	memcpy(tempBuffer, m_buffer + 4, packetSize - 4);
//	tempBuffer += packetSize - 4;
//
//	char sendBuffer[BUFFER_MAX];
//	char* sendTempBuffer = sendBuffer;
//
//	int sendLen = 2 + 2 + tempBuffer - buffer;
//
//	*(unsigned short*)sendTempBuffer = sendLen; // 2022-04-29 test
//	sendTempBuffer += sizeof(unsigned short);
//	*(unsigned short*)sendTempBuffer = CS_PT_CHATTING;
//	sendTempBuffer += sizeof(unsigned short);
//	memcpy(sendTempBuffer, buffer, tempBuffer - buffer);
//
//	for (int i = 0; i < PLAYER_MAX; i++)
//	{
//		if (m_pUser->GetInRoomUserInfo()[i].socket != 0)
//		{
//			int sendSize = send(m_pUser->GetInRoomUserInfo()[i].socket, sendBuffer, sendLen, 0);
//			if (sendSize < 0)
//			{
//				break;
//			}
//		}
//		
//	}
//}
//
//void CSession::Ready()
//{
//	m_pUser->Ready(m_socket);
//}
//
//void CSession::Start()
//{
//	m_pUser->Start();
//}
//
//void CSession::DrawCard()
//{
//	char* tempBuffer = m_buffer + 4;
//	int cardNum = *(unsigned short*)tempBuffer;
//	tempBuffer += sizeof(unsigned short);
//	int index = *(unsigned short*)tempBuffer;
//	m_pUser->DrawCard(m_socket, cardNum, index);
//
//	RoomState();
//}
//
//void CSession::TakeCard()
//{
//	m_pUser->TakeCard(m_socket);
//}
//
//void CSession::ChoiceColor()
//{
//	char* tempBuffer = m_buffer + 4;
//	int color = *(unsigned short*)tempBuffer;
//	m_pUser->ChoiceColor(m_socket, color);
//
//	RoomState();
//}
