#pragma once
#include "user.h"
#include <WinSock2.h>
#include <WS2tcpip.h>

struct fd_set_ex : fd_set {
    CUser* user_array[FD_SETSIZE];
};

#define FD_CLR_EX(fd, set) do { \
    u_int __i; \
    for (__i = 0; __i < ((fd_set_ex FAR *)(set))->fd_count ; __i++) { \
        if (((fd_set_ex FAR *)(set))->fd_array[__i] == fd) { \
            while (__i < ((fd_set_ex FAR *)(set))->fd_count-1) { \
                ((fd_set_ex FAR *)(set))->fd_array[__i] = \
                    ((fd_set_ex FAR *)(set))->fd_array[__i+1]; \
				((fd_set_ex FAR *)(set))->user_array[__i] = \
					((fd_set_ex FAR *)(set))->user_array[__i+1]; \
                __i++; \
            } \
            ((fd_set_ex FAR *)(set))->fd_count--; \
            break; \
        } \
    } \
} while(0, 0)

#define FD_SET_EX(fd, user, set) do { \
    u_int __i; \
    for (__i = 0; __i < ((fd_set_ex FAR *)(set))->fd_count; __i++) { \
        if (((fd_set_ex FAR *)(set))->fd_array[__i] == (fd)) { \
            break; \
        } \
    } \
    if (__i == ((fd_set_ex FAR *)(set))->fd_count) { \
        if (((fd_set_ex FAR *)(set))->fd_count < FD_SETSIZE) { \
            ((fd_set_ex FAR *)(set))->fd_array[__i] = (fd); \
			((fd_set_ex FAR *)(set))->user_array[__i] = (user); \
            ((fd_set_ex FAR *)(set))->fd_count++; \
        } \
    } \
} while(0, 0)

#define FD_ZERO_EX(set) (((fd_set_ex FAR *)(set))->fd_count=0)

class CServer
{
public:
private:
	SOCKET m_sockServer;
	fd_set_ex m_fdSocketInfors;
public:
	CServer();
	~CServer();

	bool Initialize(const char* _ip, int _port);
	void Cleanup();

	void Update();
	
	void Accept();
	int Recv(int _index);
private:
};