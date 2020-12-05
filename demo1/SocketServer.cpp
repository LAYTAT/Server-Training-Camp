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

#define DEFAULT_PORT 30000

int main()
{
	std::cout << "Hello World!" << std::endl;
	// create socket
	//AF_INET - ipv4; SOCK_STREAM - 流式套接字; 
	int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_fd == -1)
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
	if (bind(sock_fd, (sockaddr*)&addr, sizeof(addr)) == -1)
	{
		std::cout << "bind failed!" << std::endl;
		return 0;
	}
	//listen
	if (listen(sock_fd, 5) == -1)
	{
		std::cout << "listen failed!" << std::endl;
		return 0;
	}
	// accept
	sockaddr_in clientaddr;
	socklen_t clientaddrlen = sizeof(clientaddr);
	char clientIP[INET_ADDRSTRLEN] = "";
	while (true)
	{
		std::cout << "listening" << std::endl;
		int connid = accept(sock_fd, (sockaddr*)&clientaddr, &clientaddrlen);
		if (connid < 0)
		{
			std::cout << "accpet failed!" << std::endl;
			continue;
		}
		inet_ntop(AF_INET, &clientaddr.sin_addr, clientIP, INET_ADDRSTRLEN);
		std::cout << "connecting : " << clientIP << ":" << ntohs(clientaddr.sin_port) << std::endl;
		
		char buffer[255];
		while (true)
		{
			memset(buffer, 0, sizeof(buffer));
			int len = recv(connid, buffer, sizeof(buffer), 0);
			buffer[len] = '\0';
			if (strcmp(buffer, "exit") == 0)
			{
				std::cout << "disconnecting : " << clientIP << ":" << ntohs(clientaddr.sin_port) << std::endl;
				break;
			}
			std::cout << buffer << std::endl;
			send(connid, buffer, len, 0);

		}
		close(connid);
	}
	close(sock_fd);
	return 0;
}
