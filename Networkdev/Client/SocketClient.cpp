#include "../include/SocketClient.h"
#include "../protocal/MsgTest.pb.h"
#include "../protocal/MSG_PLAYER_MOVE.pb.h"
#include "../protocal/MSG_PLAYER_REGISTER.pb.h"

SocketClient::SocketClient()
{
    this->m_ListenSock = new baselink();
    this->m_msg_head  = new MesgHead();
}

SocketClient::~SocketClient()
{
}


bool SocketClient::Init(INT32 port)
{
    m_linkmap.clear();
    if (m_ListenSock->Init(-1) == false)
    {
        std::cout << "socket init failed" << std::endl;
        return false;
    }
    if (m_ListenSock->OpenClient(port) == -1)
    {
        std::cout << "Open client failed!" << std::endl;
    }
    m_basefd = m_ListenSock->GetFD();
    if (m_ListenSock->ConnectServer() == -1)
    {
        std::cout << "Connect to server failed!" << std::endl;
        //todo , closesocket
        return false;
    }
    m_linkmap[m_basefd] = new baselink();
    m_linkmap[m_basefd]->Init(m_basefd);

    m_Epfd = epoll_create(MAX_LINK_COUNT);
    ev.events = EPOLLIN;
	ev.data.fd = m_ListenSock->GetFD();
	if (epoll_ctl(m_Epfd, EPOLL_CTL_ADD, m_basefd, &ev) == -1)
	{
		std::cout << "Epoll add ev failed!" << std::endl;
		return false;
	}
    
    ev.data.fd = STDIN_FILENO;
    if (epoll_ctl(m_Epfd, EPOLL_CTL_ADD, STDIN_FILENO, &ev) == -1)
	{
		std::cout << "Can't create socket listen stdin!" << std::endl;
		return false;
	}

    //todo msgid->golbalconfig uid
    if (m_msg_head->Init(1005,998,0) == false)
    {
        std::cout << "m_msg_head is wrong" << std::endl;
        return false;
    }
    
    return true;
}

void SocketClient::Uinit()
{
    for(auto it = m_linkmap.begin(); it != m_linkmap.end(); it++)
    {
        delete it->second;
    }
    m_linkmap.clear();
    m_ListenSock->Uinit();
}

void SocketClient::Dojob()
{
    while (true)
    {
        size_t ready_size = epoll_wait(m_Epfd, events, MAX_LINK_COUNT, -1);
		for (int i = 0; i < ready_size; i++)
		{
            if (events[i].data.fd == STDIN_FILENO)   //键盘输入
			{
                // define buffer (static)
                static char head_buffer[HEADBUFFER_SIZE+1];   //pack head
                static char msg_buffer[BUFFER_SIZE+1];   //pack body == msg

                // get input
                std::cin.getline(msg_buffer,BUFFER_SIZE);
                int len = strlen(msg_buffer);
                if (len == 0)
                {
                    continue;
                }
                msg_buffer[len] = '\0';
                

                // set msg 
                /*
                MsgTest t_msg;
                t_msg.set_id(1005);
                t_msg.set_uid(m_msg_head->GetuID());
                t_msg.set_msg(msg_buffer);
                INT32 t_msg_len = t_msg.ByteSizeLong();
                t_msg.SerializeToArray( msg_buffer, t_msg_len );
                */
                MSG_PLAYER_REGISTER t_msg_player;
                t_msg_player.set_x(100);
                t_msg_player.set_z(121);
                t_msg_player.set_ry(1);
                t_msg_player.set_speed(20);
                t_msg_player.set_professsional(1);
                t_msg_player.set_playerid(10077);
                INT32 t_msg_player_len = t_msg_player.ByteSizeLong();



                // std::cout << "protobuf send: --------------------------" << std::endl;
                // std::cout << "msgid: " << t_msg.id() << std::endl;
                // std::cout << "uid: " << t_msg.uid() << std::endl;
                // std::cout << "msg: " << t_msg.msg() << std::endl;
                // std::cout << "msglns: " << t_msg_len << std::endl;
                // std::cout << "protobuf send: --------------------------" << std::endl;

                //send msg
                /*
                int headlen = m_msg_head->encode(head_buffer, t_msg_len); //encode msghead & set packlen = len
                m_ListenSock->SendData(head_buffer, headlen);
                m_ListenSock->SendData(msg_buffer, t_msg_len);
                */
                //move test
                t_msg_player.SerializeToArray(msg_buffer,t_msg_player_len);
                MesgHead t_msg_player_head ;
                t_msg_player_head.Init(1008,231,0);
                int t_headlen = t_msg_player_head.encode(head_buffer, t_msg_player_len);
                m_ListenSock->SendData(head_buffer,t_headlen);
                m_ListenSock->SendData(msg_buffer,t_msg_player_len);
                
			}
			else if (events[i].data.fd == m_basefd)  //收到数据
			{
                baselink*& t_linker = m_linkmap[m_basefd]; 
                if (t_linker == 0)
                {
                    std::cout << "Can't find this sockfd in m_linmap , m_basefd = " << m_basefd << std::endl;
                }
                if(t_linker->RecvData() <= 0)
                {
                    continue;
                }

                INT32 t_buf_size = t_linker->GetBufferSize();
                INT32 t_msghead_size = m_msg_head->GetMsgHeadSize();
                while ( t_msghead_size <= t_buf_size )
                {
                    MesgInfo t_msginfo = m_msg_head->decode(t_linker->GetBufferHead());
                    std::cout << "t_msginfo.msgid = " << t_msginfo.msgID  << std::endl;
                    std::cout << "t_msginfo.uid = " << t_msginfo.uID  << std::endl;
                    std::cout << "t_msginfo.packlen = " << t_msginfo.packLen  << std::endl;
                    INT32 t_pack_len = t_msginfo.packLen + t_msghead_size;
                    if (t_pack_len > t_buf_size)
                    {
                        std::cout << "Pack have not been reveived completed, Pack'len = " << t_pack_len << " ,  buffer len = " << t_buf_size << std::endl;
                        break;
                    }

                    char *str = t_linker->GetPack(t_pack_len);
                    MsgTest t_msg;
                    t_msg.ParseFromArray( &str[t_msghead_size], t_msginfo.packLen );
                    std::cout << "protobuf receive: --------------------------" << std::endl;
                    std::cout << "msgid: " << t_msg.id() << std::endl;
                    std::cout << "uid: " << t_msg.uid() << std::endl;
                    std::cout << "msg: " << t_msg.msg() << std::endl;
                    std::cout << "protobuf receive: --------------------------" << std::endl;

                    t_buf_size = t_linker->GetBufferSize();
                    t_msghead_size = m_msg_head->GetMsgHeadSize();
                }
                
			}
		}
    }
    return ;
}
