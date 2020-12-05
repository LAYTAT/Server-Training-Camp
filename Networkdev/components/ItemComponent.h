#ifndef _ITEMCOMPONENT_H_
#define _ITEMCOMPONENT_H_

#include "../include/util.h"
#include "../include/BaseComponent.h"
#include "../include/BagContainer.h"
#include "../include/EquipContainer.h"
#include "../include/CurrencyContainer.h"

class ItemContainerComponent : public BaseComponent
{
public:
    ItemContainerComponent(){}
    ~ItemContainerComponent(){}
    FUNCCLONE(ItemContainerComponent)
    
    BagContainer m_bagContainer;  // 背包容器
    EquipContainer m_equipContainer;  // 装备容器
    CurrencyContainer m_curContainer;  // 货币容器（写全名可太长了QAQ）
    
};

#endif
