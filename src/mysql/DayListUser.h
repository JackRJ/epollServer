/*
 * @Author: your name
 * @Date: 2020-05-18 17:33:59
 * @LastEditTime: 2020-06-02 16:33:56
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /try/src/mysql/DayListUser.h
 */ 
#include <mysql/mysql.h>
#include <stdio.h>
#include <string>
#include <map>

class DayListUser
{
private:
    // 记录连接状态
    bool connected;
    MYSQL conn;
    MYSQL_RES *result = NULL;
    MYSQL_FIELD *field = NULL;
public:
    int login(std::map<std::string, std::string>& bodiesr);
    int registe(std::string account, std::string cipher);
    DayListUser();
    ~DayListUser();
};
