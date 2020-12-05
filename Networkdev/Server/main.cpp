#include "../include/util.h"
#include "../include/Singleton.h"
#include "../include/SocketServer.h"
#include "../include/PlayerMgr.h"
#include "../include/EventSystem.h"
#include "../include/ComponentsMgr.h"
#include "../include/EntityMgr.h"
#include "../include/ItemsMgr.h"
#include "../protocal/GameSpec.pb.h"
#include "../include/MySqlMgr.h"
#include "../include/CacheMgr.h"

void SglCreater()
{
    CREATE_SINGLETON(SocketServer)
    CREATE_SINGLETON(PlayerMgr)
    CREATE_SINGLETON(EventSystem)
    CREATE_SINGLETON(ComponentsMgr)
    CREATE_SINGLETON(EntityMgr)
    CREATE_SINGLETON(ItemsMgr)
    CREATE_SINGLETON(MySqlMgr)
    CREATE_SINGLETON(CacheManager)
    
}

void SglDestoryer()
{
    DESTORY_SINGLETON(CacheManager)
    DESTORY_SINGLETON(MySqlMgr)
    DESTORY_SINGLETON(SocketServer)
    DESTORY_SINGLETON(PlayerMgr)
    DESTORY_SINGLETON(EventSystem)
    DESTORY_SINGLETON(ComponentsMgr)
    DESTORY_SINGLETON(EntityMgr)
    DESTORY_SINGLETON(ItemsMgr)
}

int main()
{   
    // create singleton
    SglCreater();
    //--------------------------------------------------//
    SocketServer::Instance()->Dojob();
    //--------------------------------------------------//
    // destory singleton
    SglDestoryer();
    return 0;
}
