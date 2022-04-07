#pragma once
#include "session.h"
#include "Information.h"
#include "Room.h"
#include "user.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <list>

struct fd_set_ex : fd_set {
    CSession* session_array[FD_SETSIZE];
};

#define FD_CLR_EX(fd, set) do { \
    u_int __i; \
    for (__i = 0; __i < ((fd_set_ex FAR *)(set))->fd_count ; __i++) { \
        if (((fd_set_ex FAR *)(set))->fd_array[__i] == fd) { \
            while (__i < ((fd_set_ex FAR *)(set))->fd_count-1) { \
                ((fd_set_ex FAR *)(set))->fd_array[__i] = \
                    ((fd_set_ex FAR *)(set))->fd_array[__i+1]; \
				((fd_set_ex FAR *)(set))->session_array[__i] = \
					((fd_set_ex FAR *)(set))->session_array[__i+1]; \
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
			((fd_set_ex FAR *)(set))->session_array[__i] = (user); \
            ((fd_set_ex FAR *)(set))->fd_count++; \
        } \
    } \
} while(0, 0)

#define FD_ZERO_EX(set) (((fd_set_ex FAR *)(set))->fd_count=0)

class CSelect
{
public:
    typedef std::list<CRoom*> RoomList;
    typedef std::list<CUser*> UserList;

    enum TYPE
    {
        NICK_NAME = 1,
        CREATE_ROOM,
        USERLIST,
        ROOMLIST,
        MAX
    };

private:
    SOCKET m_listenSocket;
    fd_set_ex m_fdSocketInfors;

public:
	CSelect();
	~CSelect();
    CSelect(SOCKET _listenSocket);

    void Update();
    void Accept();

    void remove(CSession* _session);
private:
};