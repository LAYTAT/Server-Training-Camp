#include "../include/SocketClient.h"
int main()
{   
    SocketClient* client = new SocketClient();
    std::cout << "please input port :" ;
    int port;
    std::cin>>port;
    if(client->Init(port) == false)
    {
        return 0;
    }
    client->Dojob();

    return 0;
}