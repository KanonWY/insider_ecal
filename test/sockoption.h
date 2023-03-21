#ifndef __SOCKET_OPTION_H__
#define __SOCKET_OPTION_H__

#include <iostream>
#include <sys/socket.h> 
#include <errno.h>

void  dumpSocketBuffer(int fd)
{
    socklen_t optlen;
    int snd_size = 0;
    int err = -1;

    optlen = sizeof(snd_size);
    err = getsockopt(fd, SOL_SOCKET, SO_SNDBUF, &snd_size, &optlen);

    if(err < 0)
    {
        std::cout << "get sndbuffer size error" << std::endl;
        return;
    }
    else
    {
        std::cout << "sndbuffer size ==> " << snd_size << std::endl;
    }
}


void setSndBufferSize(int fd, int size)
{
    socklen_t optlen;
    int err = -1;

    optlen = sizeof(size);
    err = setsockopt(fd, SOL_SOCKET, SO_SNDBUF, &size, optlen);

    if(err < 0)
    {
        std::cout << "set sndbuffer size error" << std::endl;
        return;
    }
    else
    {
        std::cout << "sndbuffer size ==> " << size << std::endl;
    }
}



#endif