/*
 * @Author: your name
 * @Date: 2020-05-18 21:47:43
 * @LastEditTime: 2020-06-17 17:24:00
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
