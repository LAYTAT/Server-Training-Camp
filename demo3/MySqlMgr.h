//
// Created by Christopher  on 2020/11/25.
//

#ifndef FORTHWEEK_MYSQLMGR_H
#define FORTHWEEK_MYSQLMGR_H
#include "Singleton.h"
#include "util.h"
#include "mysql++/mysql++.h"
#include "mysql/mysql.h"


class MySqlMgr {
private:
    MySqlMgr();
    ~MySqlMgr();
    DECLARE_SINGLETON(MySqlMgr)
    mysqlpp::Connection conn;
    bool updateFun(const std::string& s);
    bool searchFun(const std::string& s,std::string& res);
    bool insertFun(const std::string& s);

public:
    bool Init();
    void Uinit();
    bool UpdateName(int id,const std::string& newName);
    bool AddPlayer(int id,const std::string& newName);
    bool GetPlayerName(int id,std::string& resName);
    bool UpdatePlayerInfo(int id,const char* info,int len); //BLOB
};


#endif //FORTHWEEK_MYSQLMGR_H
