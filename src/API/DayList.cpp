/*
 * @Author: your name
 * @Date: 2020-05-18 21:47:43
 * @LastEditTime: 2020-06-21 16:52:14
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /try/API/DayListUser.cpp
 */ 
#include "DayList.h"
#include <memory>

/**
 * 用户登陆
 * @params:account, cipher
 */
int loginAPI(std::map<std::string, std::string>& bodies)
{
    std::shared_ptr<DayListUser> user(new DayListUser());
    return user -> login(bodies);
}

/**
 * 用户注册
 * @params:account, cipher
 */
int registeAPI(std::map<std::string, std::string>& bodies)
{
    std::shared_ptr<DayListUser> user(new DayListUser());
    if (!bodies.count("account") || !bodies.count("cipher"))
        return -1;
    return user -> registe(bodies["account"], bodies["cipher"]);
}

/**
 * 用户上传日程
 * @params:userId, startTime, endTime, isAlarm, advancedAlarmMintes, location, describtion, remarks
 */
int uploadScheduleItemAPI(std::map<std::string, std::string>& bodies)
{
    if (!bodies.count("userId") || !bodies.count("startTime") 
        || !bodies.count("endTime") || !bodies.count("describtion"))
        return -1;
    if (bodies.count("isAlarm") && !bodies.count("advancedAlarmMintes"))
        return -1;
    std::shared_ptr<DayListUser> user(new DayListUser());
    return user -> uploadScheduleItem(bodies);
}

/**
 * 获取用户日程，每次10条
 * @params:userId, page
 */
int getUserItem(std::map<std::string, std::string>& urlData, std::string& items)
{
    if (!urlData.count("userId") || !urlData.count("page"))
        return -1;
    std::shared_ptr<DayListUser> user(new DayListUser());
    return user -> getUserItem(urlData, items);
}
