#ifndef _COMPONENTSMGR_H_
#define _COMPONENTSMGR_H_

// 所有component的注册类
// 方便程序new一个对应的component
#include "util.h"
#include "Singleton.h"
#include "BaseComponent.h"

class ComponentsMgr
{
private:
    ComponentsMgr();
    ~ComponentsMgr();
    DECLARE_SINGLETON(ComponentsMgr)
public:
    bool Init();
    void Uinit();
    BaseComponent* GetComponent(INT32 comtypeid);
    

private:
    std::unordered_map<INT32, BaseComponent*> m_ComponentsMap;

};
#endif
