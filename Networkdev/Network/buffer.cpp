#include "../include/buffer.h"

buffer::buffer()
{
}

buffer::~buffer()
{
}

bool buffer::Init(INT32 sockid)
{
    m_phead = 0;
    m_ptail = 0;
    m_psocketfd = sockid;

    memset(m_pbuffer, 0x0, sizeof(char)*(BUFFER_SIZE+1));
    return true;
}

void buffer::Uinit()
{
    
}

INT32 buffer::CheckAndReset()
{
    if (m_ptail == BUFFER_SIZE)
    {
        if (m_phead == 0)
        {
            return 0;
        }
        else
        {
            memmove(&m_pbuffer[0], &m_pbuffer[m_phead], m_ptail - m_phead );
            m_phead = 0;
            m_ptail = m_ptail - m_phead;
        }
    }
    return BUFFER_SIZE-m_ptail;
}

INT32 buffer::RecvData()
{
    INT32 len = CheckAndReset();
    if (len == 0)
    {
        return 0;
    }
    INT32 ret = recv(m_psocketfd, this->GetTail(), len, 0);
    //std::cout << "received data's len = " << ret << "   from sockfd = "<< m_psocketfd << std::endl;

    if( ret >0 )
    {
        m_ptail += ret;
    }
    else
    {
        if((ret<0) &&(errno == EAGAIN||errno == EWOULDBLOCK||errno == EINTR)) //这几种错误码，认为连接是正常的，继续接收
        {
            std::cout << "ret< 0 || errno , ret = "<< ret<<"   errno = "<<errno<<std::endl;
            return 0;
        }

        std::cout << "ret == 0"<< ret<<"   errno = "<<errno<<std::endl;
        //CloseSocket();
        return -1;
    }
    return ret;
}

