#ifndef _PLAYERMGR_H_
#define _PLAYERMGR_H_

#include "util.h"
#include "Player.h"
#include "Singleton.h"

class PlayerMgr
{
private:
    PlayerMgr(); 
    ~PlayerMgr();
    DECLARE_SINGLETON(PlayerMgr)

public:
    bool Init();
    void Uinit();
    bool RegisterPlayer(INT32 uid);
    Player* GetPlayerByID(INT32 uid){ return m_players[uid]; }


private:
    //Players Map: uid -> Player*
    std::map<INT32, Player*> m_players;
    


};
#endif
