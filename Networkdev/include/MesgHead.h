#ifndef _MESGHEAD_H_
#define _MESGHEAD_H_

#include "util.h"

struct MesgInfo{
    INT32 msgID;
    INT32 uID;
    INT32 packLen;
};

class MesgHead
{
public:
    MesgHead();
    ~MesgHead();
    
    bool Init(INT32 msg, INT32 uid, INT32 lens);
    void Uinit();

    const INT32 GetmsgID(){ return msgheader.msgID; };
    const INT32 GetuID(){ return msgheader.uID; };
    const INT32 GetMsgHeadSize();

    INT32 encode(char* buffer, INT32 len);
    MesgInfo decode(char * buffer);
private:
    MesgInfo msgheader;

};

#endif