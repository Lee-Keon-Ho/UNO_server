#pragma once
#include "session.h"

struct fd_set_ex : fd_set {
    CSession* session_array[FD_SETSIZE];
};

#define FD_CLR_EX(fd, set) do { \
    u_int __i; \
    for (__i = 0; __i < ((fd_set_ex FAR *)(set))->fd_count ; __i++) { \
        if (((fd_set_ex FAR *)(set))->fd_array[__i] == fd) { \
            delete ((fd_set_ex FAR *)(set))->session_array[__i]; \
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