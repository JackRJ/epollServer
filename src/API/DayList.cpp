/*
 * @Author: your name
 * @Date: 2020-05-18 21:47:43
 * @LastEditTime: 2020-05-18 21:56:05
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /try/API/DayListUser.cpp
 */ 
#include "DayList.h"

int login(std::map<std::string, std::string> bodies)
{
    DayListUser dayListUser;
    if (!bodies.count("account") || !bodies.count("cipher"))
        return 0;
    if (dayListUser.login(bodies["account"], bodies["cipher"]) == 1)
        return 1;
    return 0;
}
