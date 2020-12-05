#include "../include/baselink.h"

baselink::baselink()
{
	m_buffer = nullptr;
}

baselink::~baselink()
{
    Uinit();
}

bool baselink::Init(INT32 sock)
{
    m_socketfd = sock;
    if (m_buffer == nullptr)
    {
        m_buffer = new buffer();
    }
    return m_buffer->Init(sock);
}

void baselink::Uinit()
{
    CloseSocket();
    if(m_buffer != nullptr)
    {
        m_buffer->Uinit();
        delete m_buffer;
        m_buffer = nullptr;
    }
}

INT32 baselink::OpenServer(INT32 port, char* IP)
{
    if (m_socketfd > 0)
    {
        CloseSocket();
    }
    //| SOCK_NONBLOCK
    m_socketfd = socket(AF_INET, SOCK_STREAM , 0);
    if (m_socketfd < 0)
    {
        std::cout << "Open server socket failed!" << std::endl;
        m_socketfd = -1;
        return -1;
    }
    std::cout <<"Open server sock success , server sock = " << m_socketfd << std::endl;
    
    
    int flags = 1;
    struct linger ling = {0, 0};
    if(0 != setsockopt(m_socketfd, SOL_SOCKET, SO_KEEPALIVE, &flags, sizeof(flags)))
    {
        CloseSocket();
    }
    if(0 != setsockopt(m_socketfd, SOL_SOCKET, SO_LINGER, &ling, sizeof(ling)))
    {

        printf("setsockopt failed with code[%d]\n", errno);
        CloseSocket();
        return -1;   
    }
    if(0 != setsockopt(m_socketfd, IPPROTO_TCP, TCP_NODELAY, &flags, sizeof(flags))) //set TCP_CORK
    {

        printf("setsockopt failed with code[%d]\n", errno);
        CloseSocket();
        return -1;   
    }
    if (0 != setsockopt(m_socketfd, SOL_SOCKET, SO_REUSEADDR, &flags, (int)sizeof(flags))){
        printf("setsockopt failed with code[%d]\n", errno);
        CloseSocket();
        return -1;
    }   



    sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (IP == NULL)
    {
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
    }
	else 
    {
        addr.sin_addr.s_addr = inet_addr(IP);  //INADDR_ANY就是指定地址为0.0.0.0的地址
    }
    socklen_t addrSize = socklen_t(sizeof(addr));
	if (bind(m_socketfd, (sockaddr*)&addr, addrSize) == -1)
	{
		std::cout << "server socket bind failed!" << std::endl;
        CloseSocket();
		return -1;
	}
	//listen
	if (listen(m_socketfd, MAX_LINK_COUNT) == -1)
	{
		std::cout << "server socket listen failed!" << std::endl;
        CloseSocket();
		return -1;
	}
    //设置为非阻塞
    if(set_non_block(m_socketfd) == -1)
    {
        std::cout << "Set socket non block is failed! sockfd = " << m_socketfd <<std::endl;     
        CloseSocket();
        return -1;
    }
    return 0;
}

INT32 baselink::set_non_block( INT32 fd ) {
    int flag = 0;
    flag = fcntl(fd, F_GETFL, 0);
    if (flag < 0) {
        return -1;
    }

    flag |= O_NONBLOCK;
    flag |= O_NDELAY;

    if (fcntl(fd, F_SETFL, flag) < 0) {
        return -1;
    }

    return 0;
}

INT32 baselink::OpenClient(INT32 port)
{
    if (m_socketfd > 0)
    {
        CloseSocket();
    }
    //| SOCK_NONBLOCK
    m_socketfd = socket(AF_INET, SOCK_STREAM , 0);
    if (m_socketfd < 0)
    {
        std::cout << "Open server socket failed!" << std::endl;
        m_socketfd = -1;
        return -1;
    }

    std::cout << "Open server client success , client sock :" << m_socketfd << std::endl;

    int32_t iOptValue = 1;
    if (0 != setsockopt(m_socketfd, SOL_SOCKET, SO_REUSEADDR, &iOptValue, sizeof(iOptValue))){
        CloseSocket();
        return -1;
    }
    if(0 != setsockopt(m_socketfd, IPPROTO_TCP, TCP_NODELAY, &iOptValue, sizeof(iOptValue))) //set TCP_CORK
    {
        CloseSocket();
        return -1;
    }
    // if (0 != setsockopt(m_socketfd, SOL_SOCKET, SO_SNDBUF, (const void *)&buffLen, sizeof(buffLen))){
    //     return -1;
    // }
    // if (0 != setsockopt(m_socketfd, SOL_SOCKET, SO_RCVBUF, (const void *)&buffLen, sizeof(buffLen))){
    //     return -1;
    // }

    struct sockaddr_in stSocketAddr;
    memset(&stSocketAddr, 0x0, sizeof(stSocketAddr));
    stSocketAddr.sin_family = AF_INET;
    // if (NULL != ip){
    //     stSocketAddr.sin_addr.s_addr = inet_addr(ip);
    // }
    // else{
        stSocketAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    // }
    stSocketAddr.sin_port = (u_short)htons(port);

    socklen_t addrSize = socklen_t(sizeof(stSocketAddr));
    if (0 != bind(m_socketfd, (const struct sockaddr*) &stSocketAddr, addrSize)){
        CloseSocket();
        return -1;
    }
    return 0;
}

INT32 baselink::ConnectServer()
{
    if (m_socketfd < 0)
    {
        std::cout << "socket fd is wrong!" << std::endl;
        return -1;
    }
    std::cout << "m_socketfd:" << m_socketfd << std::endl;
    //connect
	sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(DEFAULT_SERVER_PORT);
	serveraddr.sin_addr.s_addr = inet_addr(DEFAULT_SERVER_ADDR);
	if (connect(m_socketfd, (sockaddr*)&serveraddr, sizeof(serveraddr)) < 0)
	{
		std::cout << "clien connect failed!" << std::endl;
		return -1;
	}

	std::cout << "connect success ~.~" << std::endl;
    return 0;
}

INT32 baselink::AcceptSocket()
{
    sockaddr_in clientaddr;
    socklen_t clientaddrlen = sizeof(clientaddr);
    
    INT32 connfd = accept(m_socketfd, (struct sockaddr*)&clientaddr, &clientaddrlen);
    if(connfd <= 0)
    {
        std::cout << "Accept client socket error!" << std::endl;
        return -1;
    }
    //设置非阻塞
    if(set_non_block(connfd) == -1)
    {
        std::cout << "Set socket non block is failed! sockfd = " << m_socketfd <<std::endl;     
        CloseSocket();
        return -1;
    }

    return connfd;
}

INT32 baselink::RecvData()
{
    if (m_socketfd < 0)
    {
        return -1;
    }
    INT32 ret = m_buffer->RecvData();
    //if(ret == -1)
    //{
    //    CloseSocket();
    //}
    //在外面析构这个类的时候CloseSocket
    
    // 0代表没错误但没数据；-1代表有错误，要断开连接
    return ret;
}

INT32 baselink::SendData(char *data, INT32 len)
{
    send(m_socketfd, data, len, 0);
    
    

    return 0;
}

void baselink::CloseSocket()
{
    if (m_socketfd < 0)
    {
        std::cout << "Close client socket failed!" << std::endl;
        return ;
    }
    close(m_socketfd);
    m_socketfd = -1;
}

