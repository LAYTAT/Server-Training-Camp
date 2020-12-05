#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "util.h"
#include "MesgHead.h"

class Player
{
public:
    Player(INT32 uid); 
    ~Player(); 

    INT32 GetID();    
    INT32 test_func(MesgInfo &stHead, const char *body, const INT32 len);


private:
    INT32 m_uid;
    INT32 m_state;
    INT32 m_hp;
};
#endif 
