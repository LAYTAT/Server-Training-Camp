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


int main()
{
	std::cout << "This is client" << std::endl;
	//socket
	int clientsock = socket(AF_INET, SOCK_STREAM, 0);
	if (clientsock == -1)
	{
		std::cout << "client socket build failed!" << std::endl;
		return 0;
	}
	//connect
	sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(30000);
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	//std::cout << "connect id " << connect(clientsock, (sockaddr*)&serveraddr, sizeof(serveraddr)) << std::endl;
	if (connect(clientsock, (sockaddr*)&serveraddr, sizeof(serveraddr)) < 0)
	{
		std::cout << "clien connect failed!" << std::endl;
		return 0;
	}
	std::cout << "connecting ..." << std::endl;
	char data[255];
	char buffer[255];
	while (true)
	{
		std::cin >> data;
		send(clientsock, data, strlen(data), 0);
		if (strcmp(data, "exit") == 0)
		{
			std::cout << "client disconnect" << std::endl;
			break;
		}
		memset(buffer, 0, sizeof(buffer));
		int len = recv(clientsock, buffer, sizeof(buffer), 0);
		buffer[len] = '\0';
		std::cout << buffer << std::endl;
	}
	close(clientsock);

    return 0;
}
