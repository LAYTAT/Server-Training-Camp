#ifndef _SOCKETSERVER_H_
#define _SOCKETSERVER_H_

#include "util.h"
#include "Singleton.h"
#include "baselink.h"
#include "MesgHead.h"
#include "../protocal/MsgTest.pb.h"
using namespace google::protobuf;

class SocketServer
{
private:
    SocketServer();
    ~SocketServer();                            
    DECLARE_SINGLETON(SocketServer)
public:
    bool Init();
    void Uinit();
    void Dojob();
    void BroadCast(MesgInfo& msghead, Message& msg);
    void SendMsg(MesgInfo& msghead, Message& msg, const INT32 connfd);
private:
    INT32 m_Epfd;
    struct epoll_event ev, events[MAX_LINK_COUNT];
    baselink* m_ListenSock; 
    INT32  m_basefd;
    std::map<INT32, baselink*> m_linkmap;
    MesgHead* m_msg_head;    
};


#endif
