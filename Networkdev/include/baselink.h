#ifndef _BASELINK_H_
#define _BASELINK_H_

#include "buffer.h"

class baselink
{
public:
    baselink();
    ~baselink();

    bool Init(INT32 sock);
    void Uinit();
    void SetFD(INT32 sock){ m_socketfd = sock; }

    inline INT32 GetFD(){ return m_socketfd; }
    INT32 OpenServer(INT32 port, char* IP = NULL);
    INT32 OpenClient(INT32 port);
    INT32 ConnectServer();
    INT32 AcceptSocket();
    INT32 RecvData();
    INT32 SendData(char *data, INT32 len);

    INT32 GetBufferSize(){ return m_buffer->GetSize(); }  //Get saved datas size
    char * GetBuffer(INT32 &len){ return m_buffer->GetBuffer(len);}  //Get all data in buffer
    char * GetBufferHead(){ return m_buffer->GetHead(); }  //Get buffer head
    char * GetPack(INT32 len){ return m_buffer->GetPack(len); }  //Get one Package
    
    INT32 set_non_block(INT32 fd);

    void CloseSocket();

private:
    INT32 m_socketfd;
    buffer* m_buffer; 

};

#endif
