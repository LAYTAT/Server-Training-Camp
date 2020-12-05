#ifndef _EVENTSYSTEM_H_
#define _EVENTSYSTEM_H_

#include "util.h"
#include "Singleton.h"
#include "MsgHandler.h"
#include "EntityMgr.h"
#include <functional>
#include "../systems/BagSystem.h"

class EventSystem
{
private:
    EventSystem();
    ~EventSystem();
    DECLARE_SINGLETON(EventSystem);
public:
    bool Init();
    void Uinit();

    MsgHandler<EventSystem>*  GetMsgHandler() { return m_msgHandler; }
    
    INT32 PlayerRegister(MesgInfo &stHead, const char *body, const INT32 len,const INT32 connfd);
    INT32 PlayerMove(MesgInfo &stHead, const char *body, const INT32 len,const INT32 connfd);
    INT32 PlayerAddItem(MesgInfo &stHead, const char *body,const INT32 len,const INT32 connfd);
    INT32 PlayerRemoveItem(MesgInfo &stHead, const char *body,const INT32 len,const INT32 connfd);
    INT32 PlayerLogin(MesgInfo &stHead, const char *body,const INT32 len,const INT32 connfd);
    INT32 GetRankList(MesgInfo &stHead, const char *body,const INT32 len,const INT32 connfd);
private:
    MsgHandler<EventSystem>* m_msgHandler;

    BagSystem m_bagSystem;
};

#endif
