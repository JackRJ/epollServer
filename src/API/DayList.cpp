/*
 * @Author: your name
 * @Date: 2020-05-18 21:47:43
 * @LastEditTime: 2020-06-17 17:35:48
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /try/API/DayListUser.cpp
 */ 
#include "DayList.h"
#include <memory>

int loginAPI(std::map<std::string, std::string>& bodies)
{
    std::shared_ptr<DayListUser> user(new DayListUser());
    return user -> login(bodies);
}

int registeAPI(std::map<std::string, std::string>& bodies)
{
    std::shared_ptr<DayListUser> user(new DayListUser());
    if (!bodies.count("account") || !bodies.count("cipher"))
        return -1;
    return user -> registe(bodies["account"], bodies["cipher"]);
}
