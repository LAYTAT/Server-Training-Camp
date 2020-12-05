#include "../include/ItemsMgr.h"
#include "../include/ItemList.h"

INSTANCE_SINGLETON(ItemsMgr)

bool ItemsMgr::Init()
{
    // item注册biao
    
    m_itemRegister[ITEMID_REDPOTION] = ITEMTYPE_BAG_ITEM; //红药水 - bagitem
    
    m_itemRegister[ITEMID_SWORD] = ITEMTYPE_EQUIP_ITEM;  //剑 - equipitem

    m_itemRegister[ITEMID_COIN] = ITEMTYPE_CURRENCY_ITEM;  //金币 - currencyitem

    return true;
}

void ItemsMgr::Uinit()
{

}

INT32 ItemsMgr::GetTypeByID(INT32 itemid)
{
    return m_itemRegister[itemid];
}

//正常来说要用表判断一下，但现在有点麻烦，先不搞
//todo
BagItem* ItemsMgr::GetBagItem(INT32 itemid, INT32 cnt)
{
    //if(m_itemRegister.find(itemid) == m_itemRegister.end())
    //{
    //    return nullptr;
    //}
    return new BagItem(itemid, m_itemRegister[itemid], cnt, false);   // itemid , typeid, cnt, binded
}

EquipItem* ItemsMgr::GetEquipItem(INT32 itemid)
{
    //if(m_itemRegister.find(itemid) == m_itemRegister.end())
    //{
    //    return nullptr;
    //}
    return new EquipItem(itemid, m_itemRegister[itemid], false, 90, 100);   // itemid ,type,binded,attack,durable
}

CurrencyItem* ItemsMgr::GetCurrencyItem(INT32 itemid,INT32 cnt)
{
    //if(m_itemRegister.find(itemid) == m_itemRegister.end())
    //{
    //    return nullptr;
    //}
    return new CurrencyItem(itemid, m_itemRegister[itemid], cnt);   // itemid ,type,binded,attack,durable
}





