/*
 * @Author: your name
 * @Date: 2020-05-18 17:33:59
 * @LastEditTime: 2020-06-22 16:05:17
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /try/src/mysql/DayListUser.h
 */ 
#pragma once
#include <mysql/mysql.h>
#include <stdio.h>
#include <iconv.h>
#include <stdlib.h>
#include <locale.h>
#include <string>
#include <string.h>
#include <memory>
#include <map>
#include <vector>

class DayListUser
{
private:
    // 记录连接状态
    bool connected;
    MYSQL conn;
    MYSQL_RES *result = NULL;
    MYSQL_FIELD *field = NULL;

public:
    // 用户
    int login(std::map<std::string, std::string>& bodies, int& userId);
    int registe(std::string& account, std::string& cipher);

    // 日程表
    int uploadScheduleItem(std::map<std::string, std::string>& item);
    int getUserItem(std::map<std::string, std::string>& urlData, std::string& items, char& more);

    DayListUser();
    ~DayListUser();
};
