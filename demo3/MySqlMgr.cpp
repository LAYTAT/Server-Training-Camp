#include "MySqlMgr.h"



INSTANCE_SINGLETON(MySqlMgr)

MySqlMgr::MySqlMgr()
{
}

MySqlMgr::~MySqlMgr()
{
    Uinit();
}

bool MySqlMgr::Init()
{
    try {
        conn(mysqlpp::use_exceptions);
        auto ret = conn.connect(DATABASE_NAME, DATABASE_IP, DATABASE_USERNAME, DATABASE_PWD);
    }
    catch (mysqlpp::BadQuery er)
    { // handle any connection or
        // query errors that may come up
        std::cerr << "Error: " << er.what() << std::endl;
        return false;
    }
    catch (const mysqlpp::BadConversion& er)
    {
        // Handle bad conversions
        std::cerr << "Conversion error: " << er.what() << std::endl <<
             "\tretrieved data size: " << er.retrieved <<
             ", actual size: " << er.actual_size << std::endl;
        return false;
    }
    catch (const std::Exception& er)
    {
        // Catch-all for any other MySQL++ exceptions
        std::cerr << "Error: " << er.what() << std::endl;
        return false;
    }

    return true;

}

bool MySqlMgr::insertFun(const std::string& s)
{
    try {


        mysqlpp::Query *pQuery = &conn.query();
        std::cout << "Inserting test" << std::endl;
        *pQuery << s.c_str();
        mysqlpp::SimpleResult res = pQuery->execute();
    }catch (const std::Exception& er)
    {
        std::cout << "insert failed(" <<s << ")" <<"ERROR:" << er.what()<<std::endl;
        return false;
    }
    return true;
}

//查
bool MySqlMgr::searchFun(const std::string& s,std::string& res)
{
    /* Now SELECT */
    try {
        mysqlpp::Query *pQuery = &conn.query();
        *pQuery << s.c_str();
        mysqlpp::StoreQueryResult ares = pQuery->store();
        for (size_t i = 0; i < ares.num_rows(); i++) {
            res =ares[i]["playerName"];
        }
    }catch (const std::Exception& er)
    {
        std::cout << "searchFun failed(" <<s << ")" <<"ERROR:" << er.what()<<std::endl;
        return false;
    }
    return true;
}

bool MySqlMgr::updateFun(const std::string& s)
{
    try {
        mysqlpp::Query *pQuery = &conn.query();

        *pQuery << s.c_str();
        if (pQuery->exec())
        {
            std::cout << "updated success!" << std::endl;
        }
        }
        catch (const std::Exception& er)
        {
            std::cout << "updateFun failed(" <<s << ")" <<"ERROR:" << er.what() <<std::endl;
            return false;
        }
    return true;
}

bool MySqlMgr::UpdateName(int id,const std::string& newName) //暴露给上层接口：修改用户姓名
{
    std::string a ="update players set playerName = '" + newName + "' where id =" + std::to_string(id);
    return updateFun(a);

}

bool MySqlMgr::AddPlayer(int id,const std::string& newName)
{
    std::string a ="insert into players(id,playerName) values(" + std::to_string(id) +",'" + newName + "')";
    return insertFun(a);
}

bool MySqlMgr::GetPlayerName(int id,std::string& resName)
{
    std::string a="select playerName from players where id=" + std::to_string(id);
    return searchFun(a,resName);
}

void MySqlMgr::Uinit()
{
    delete m_singleton;
    return;
}


