#ifndef _BUFFER_H_
#define _BUFFER_H_

#include "util.h"

class buffer
{
public:
    buffer();
    ~buffer();
    
    bool Init(INT32 sockid);
    void Uinit();

    INT32 CheckAndReset();   //cheak cache, and get buffer lens which can be used
    INT32 RecvData();
    
    char * GetHead(){ return &m_pbuffer[m_phead]; }
    char * GetTail(){ return &m_pbuffer[m_ptail]; }
    INT32 GetSize(){ return m_ptail-m_phead;}  //Get saved datas size
    char * GetBuffer(INT32& len){ len = m_ptail-m_phead; m_ptail = m_phead; return &m_pbuffer[m_phead]; } //Get all datas in buffer
    char * GetPack(INT32 len){ INT32 tmp = m_phead; m_phead += len; return &m_pbuffer[tmp]; }  //Get one Package

private:
    INT32 m_psocketfd;
    INT32 m_phead;
    INT32 m_ptail;
    char m_pbuffer[BUFFER_SIZE+1];
};

#endif
