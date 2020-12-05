#ifndef _SOCKETCLIENT_H_
#define _SOCKETCLIENT_H_

#include "util.h"
#include "baselink.h"
#include "MesgHead.h"

class SocketClient
{
public:
    SocketClient();
    ~SocketClient();

public:
    bool Init(INT32 port);
    void Uinit();
    void Dojob();

private:
    INT32 m_Epfd;
    struct epoll_event ev, events[MAX_LINK_COUNT];
    baselink* m_ListenSock; 
    INT32 m_basefd;
    std::map<INT32, baselink*> m_linkmap;
    MesgHead* m_msg_head;
};


#endif
