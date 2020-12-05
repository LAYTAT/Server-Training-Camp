#include "../include/SocketServer.h"
#include "../protocal/MsgID.pb.h"
#include "../protocal/MSG_PLAYER_MOVE.pb.h"
#include "../include/PlayerMgr.h"
#include "../include/EventSystem.h"

INSTANCE_SINGLETON(SocketServer);

SocketServer::SocketServer()
{
    this->m_ListenSock = new baselink();
    this->m_msg_head = new MesgHead();
}

SocketServer::~SocketServer()
{
    delete m_msg_head;
    delete m_ListenSock;
}


bool SocketServer::Init()
{
    m_linkmap.clear();
    // base socket init
    if (m_ListenSock->Init(-1) == false)
    {
        return false;
    }
    if (m_ListenSock->OpenServer(DEFAULT_SERVER_PORT, DEFAULT_SERVER_ADDR) == -1)
    {
        std::cout << "Open server failed!" << std::endl;
        return false;
    }
    m_basefd = m_ListenSock->GetFD();

    // epoll init 
    m_Epfd = epoll_create(MAX_LINK_COUNT);
    if(m_Epfd == -1){
        return false;
    }
    ev.events = EPOLLIN;
	ev.data.fd = m_ListenSock->GetFD();
	if (epoll_ctl(m_Epfd, EPOLL_CTL_ADD, m_basefd, &ev) == -1)
	{
		std::cout << "Epoll add ev failed!" << std::endl;
		return false;
	}
    m_msg_head->Init(1,1,1);
    return true;
}

void SocketServer::Uinit()
{
    for(auto it = m_linkmap.begin(); it != m_linkmap.end(); it++)
    {
        delete it->second;
        it->second = nullptr;
    }
    m_linkmap.clear();

    //析构函数中析构
    m_msg_head->Uinit();
    m_ListenSock->Uinit();
}

void SocketServer::Dojob()
{
    std::cout << "Server start!" << std::endl;
    while (true)
    {
        INT32 ready_size = epoll_wait(m_Epfd, events, MAX_LINK_COUNT, -1);
        if(ready_size < 0){
            std::cout << "ready_size < 0 " << std::endl;
            break;
        }
        for (int i = 0; i < ready_size; i++)
		{
			if (events[i].data.fd == m_basefd) //新的链接，将新的socket加入epfd
			{
                int connfd = m_ListenSock->AcceptSocket();
                if (connfd == -1)
                {
                    std::cout <<"connfd == -1" <<std::endl;
                    continue;
                }
                m_linkmap[connfd] = new baselink();
                m_linkmap[connfd]->Init(connfd);
                
                ev.events = EPOLLIN;
                ev.data.fd = connfd;
				epoll_ctl(m_Epfd, EPOLL_CTL_ADD, connfd, &ev);
                std::cout << "New client has linked! sockfd = " << connfd << std::endl;
			}
			else
			{
				int connfd = events[i].data.fd;
                baselink*& t_linker = m_linkmap[connfd];
                if (t_linker == 0)
                {
                    std::cout << "Can't find client socket in linkmap, connfd = "<< connfd << std::endl;
                    continue;
                }
                INT32 ret = t_linker->RecvData();           
                if (ret == 0)
                {
                    continue;
                }
                else if (ret == -1)
                {
                    auto t_it = m_linkmap.find(connfd);
                    if(t_it == m_linkmap.end())
                    {
                        continue;
                    }
                    else
                    {
                        if(t_it->second != nullptr)
                        {
                            delete t_it->second;
                            t_it->second = nullptr;
                            m_linkmap.erase(t_it);
                        }
                    }
                    std::cout << "CloseSocket used!" << std::endl;
                    // 前面已经确认过connfd一定存在
                    continue;
                }

                
                //send package
                //buffer存的所有的数据的大小
                INT32 t_buf_size = t_linker->GetBufferSize();
                //包头大小
                INT32 t_msghead_size = m_msg_head->GetMsgHeadSize();
                while ( t_msghead_size <= t_buf_size )
                {
                    //解出第一个包头
                    MesgInfo t_msginfo = m_msg_head->decode(t_linker->GetBufferHead());
                    //std::cout << "t_msginfo.msgid = " << t_msginfo.msgID  << std::endl;
                    //std::cout << "t_msginfo.uid = " << t_msginfo.uID  << std::endl;
                    //std::cout << "t_msginfo.packlen = " << t_msginfo.packLen  << std::endl;
                    
                    //整个包的长度
                    INT32 t_pack_len = t_msginfo.packLen + t_msghead_size;
                    if (t_pack_len > t_buf_size)
                    {
                        std::cout << "Pack have not been reveived completed, Pack'len = " << t_pack_len << " ,  buffer len = " << t_buf_size << std::endl;
                        break;
                    }
                    //获得一整个包，注意这里才改变buffer的头指针
                    char *str = t_linker->GetPack(t_pack_len);
                    //判断第一个包头的信息
                    if(t_msginfo.msgID == MSGID::MSG_TEST_ID)
                    {
                        MsgTest t_msg;
                        t_msg.ParseFromArray( &str[t_msghead_size], t_msginfo.packLen );
                        std::cout << "protobuf receive: --------------------------" << std::endl;
                        std::cout << "msgid: " << t_msg.id() << std::endl;
                        std::cout << "uid: " << t_msg.uid() << std::endl;
                        std::cout << "msg: " << t_msg.msg() << std::endl;
                        std::cout << "protobuf receive: --------------------------" << std::endl;
                    }
                    else
                    {
                        //获得msgID绑定的函数，然后来处理对应的MSG
                        auto func = EventSystem::Instance()->GetMsgHandler()->GetMsgFunc(t_msginfo.msgID);
                        if (NULL != func)
                        {
                            //调用处理函数
                            (EventSystem::Instance()->*func)(t_msginfo, &str[t_msghead_size], t_msginfo.packLen, connfd);
                        }
                    }
                    //重置两个值，保证循环的进行
                    t_buf_size = t_linker->GetBufferSize();
                    t_msghead_size = m_msg_head->GetMsgHeadSize();
                    

                }
			}
		}
    }
}

void SocketServer::BroadCast(MesgInfo& msghead,Message& msg)
{
    MesgHead t_mh ;
    static char s_head_buffer[1024];
    static char s_buffer[1024];
    t_mh.Init(msghead.msgID , msghead.uID, msghead.packLen);
    //std::cout << "msg id " << msghead.msgID <<" uid "<<msghead.uID << std::endl;
    INT32 t_hl = t_mh.encode(s_head_buffer, msg.ByteSizeLong());

    msg.SerializeToArray(s_buffer, msg.ByteSizeLong());

    for(auto it = m_linkmap.begin();it!=m_linkmap.end();it++)
    {
       it->second->SendData(s_head_buffer, t_hl);
       it->second->SendData(s_buffer,msg.ByteSizeLong());
    }

    return ;
}

void SocketServer::SendMsg(MesgInfo& msghead,Message& msg,const INT32 connfd)
{
    if(m_linkmap.find(connfd) == m_linkmap.end())
    {
        std::cout << "Can't find connfd to send message. connfd = " << connfd << std::endl;
        return ;
    }
    MesgHead t_mh ;
    static char s_head_buffer[1024];
    static char s_buffer[1024];
    t_mh.Init(msghead.msgID , msghead.uID, msghead.packLen);
    //std::cout << "msg id " << msghead.msgID <<" uid "<<msghead.uID << std::endl;
    INT32 t_hl = t_mh.encode(s_head_buffer, msg.ByteSizeLong());

    msg.SerializeToArray(s_buffer, msg.ByteSizeLong());

    m_linkmap[connfd]->SendData(s_head_buffer, t_hl);
    m_linkmap[connfd]->SendData(s_buffer,msg.ByteSizeLong());

    return ;
}

