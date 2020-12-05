#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include<sys/socket.h>
#include<sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#define DEFAULT_PORT 30000
#define MAXLEN 1024
#define MAX_OPEN_FD 1024


int main()
{
	struct epoll_event ev, events[MAX_OPEN_FD];
	struct sockaddr_in clientaddr;
	socklen_t clientaddrlen = sizeof(clientaddr);

	// create socket
	//AF_INET - ipv4; SOCK_STREAM - 流式套接字; 
	int listened_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (listened_fd == -1)
	{
		std::cout << "create socket build failed!" << std::endl;
		return 0;
	}
	// bind to port
	sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);  //INADDR_ANY就是指定地址为0.0.0.0的地址
	addr.sin_port = htons(DEFAULT_PORT);
	if (bind(listened_fd, (sockaddr*)&addr, sizeof(addr)) == -1)
	{
		std::cout << "bind failed!" << std::endl;
		return 0;
	}
	//listen
	if (listen(listened_fd, 20) == -1)
	{
		std::cout << "listen failed!" << std::endl;
		return 0;
	}
	// create epoll
	int epfd = epoll_create(MAX_OPEN_FD);
	ev.events = EPOLLIN;
	ev.data.fd = listened_fd;
	if (epoll_ctl(epfd, EPOLL_CTL_ADD, listened_fd, &ev) == -1)
	{
		std::cout << "Epoll add ev failed!" << std::endl;
		return 0;
	}

	char buffer[MAXLEN] = "";

	while (true)
	{
		size_t ready_size = epoll_wait(epfd, events, MAX_OPEN_FD, -1);
		for (int i = 0; i < ready_size; i++)
		{
			if (events[i].data.fd == listened_fd) //新的链接，将新的socket加入epfd
			{
				int connfd = accept(listened_fd, (struct sockaddr*)&clientaddr, &clientaddrlen);
				ev.events = EPOLLIN;
				ev.data.fd = connfd;
				epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &ev);

			}
			else
			{
				int connfd = events[i].data.fd;
				int len = read(connfd, buffer, MAXLEN);

				buffer[len] = '\0';
				if (strcmp(buffer, "exit") == 0)
				{
					std::cout << "disconnect !" << std::endl;
					epoll_ctl(epfd, EPOLL_CTL_DEL, connfd, NULL);
					close(connfd);
					//std::cout << "disconnecting : " << events[i].data << ":" << ntohs(clientaddr.sin_port) << std::endl;
					
				}
				std::cout << buffer << std::endl;
				send(connfd, buffer, len, 0);
			}
		}
	}

	//close(epfd);
	return 0;
}
