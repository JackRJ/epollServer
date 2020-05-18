/*
 * @Author: your name
 * @Date: 2020-05-18 17:34:27
 * @LastEditTime: 2020-05-18 18:27:00
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /try/src/mysql/DayListUser.cpp
 */ 
#include "DayListUser.h"

DayListUser::DayListUser():
    connected(0)
{
    // 初始化数据库
    mysql_init(&conn);
    auto tmp = mysql_real_connect(&conn, "jack", "lovezrj", "day_list_user",0,NULL, NULL, CLIENT_FOUND_ROWS);
    if (tmp)
        connected = 1;
}

DayListUser::~DayListUser()
{
    // 关闭数据库
    if (connected)
        mysql_close(&conn);
}

int DayListUser::login(std::string account, std::string cipher)
{
    if (!connected)
        return -1;
    std::string str = "select cipher from User where account = '" + account + "'";
    MYSQL_RES *result = NULL;
    MYSQL_FIELD *field = NULL;
    int res = mysql_query(&conn, str.c_str());
    if (res)
        return 0;
    result = mysql_store_result(&conn);
    MYSQL_ROW row = mysql_fetch_row(result);
    if (!row)
        return 0;
    if (cipher == std::string(row[0]))
        return 1;
    return 0;
}