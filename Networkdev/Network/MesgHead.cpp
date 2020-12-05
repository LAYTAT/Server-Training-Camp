#include "../include/MesgHead.h"
#include <stdint.h>

MesgHead::MesgHead()
{

}

MesgHead::~MesgHead()
{

}

bool MesgHead::Init(INT32 msg, INT32 uid, INT32 lens)
{
    if (msg < 0 || uid < 0 || lens < 0)
    {
        std::cout << "MesgHead init failed " << std::endl;
        return false;
    }
    msgheader.msgID = msg;
    msgheader.uID = uid;
    msgheader.packLen = lens;

    return true;
}

void MesgHead::Uinit()
{
    msgheader.msgID = 0;
    msgheader.uID = 0;
    msgheader.packLen = 0;
}

INT32 MesgHead::encode(char* buffer, INT32 len)
{
    
    uint32_t tmp = htonl(msgheader.msgID);
    memcpy(buffer, &tmp, sizeof(INT32));    
    buffer += sizeof(INT32);

    tmp = htonl(msgheader.uID);
    memcpy(buffer, &tmp, sizeof(INT32));    
    buffer += sizeof(INT32);

    msgheader.packLen = len;
    tmp = htonl(msgheader.packLen);
    memcpy(buffer, &tmp, sizeof(INT32)); 
    buffer += sizeof(INT32);
    return 3*sizeof(INT32);
}
MesgInfo MesgHead::decode(char* buffer)
{
    MesgInfo mhead;
    INT32 tmp = 0;
	memcpy(&tmp, buffer, sizeof(INT32));
    mhead.msgID = ntohl(tmp);
    buffer += sizeof(INT32);

    memcpy(&tmp, buffer, sizeof(INT32));
    mhead.uID = ntohl(tmp);
    buffer += sizeof(INT32);

    memcpy(&tmp, buffer, sizeof(INT32));
    mhead.packLen = ntohl(tmp);
    buffer += sizeof(INT32);

    return mhead;
}

const INT32 MesgHead::GetMsgHeadSize()
{
    return 3*sizeof(INT32);
}
