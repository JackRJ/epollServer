/*
 * @Author: your name
 * @Date: 2020-05-18 21:47:43
 * @LastEditTime: 2020-06-26 15:55:43
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
int loginAPI(map<string, string>& headers_, map<string, string>& bodies, int& userId)
{
    // 验证cookie是否正确
    if (headers_.count("Cookie") && bodies.count("account"))
    {
        shared_ptr<DayListUser> user(new DayListUser());
        int userId = user -> getUserId(bodies["account"]);
        printf("userId: %i\n", userId);
    }
    if (!bodies.count("account") || !bodies.count("cipher"))
        return -1;
    // 验证账号密码长度是否符合要求
    int len = bodies["account"].size();
    if (len != 10 || (bodies["cipher"].size() < 8 || bodies["cipher"].size() > 20))
        return -1;
    
    shared_ptr<DayListUser> user(new DayListUser());
    return user -> login(bodies, userId);
}

/**
 * 用户注册
 * @params:account, cipher
 */
int registeAPI(map<string, string>& bodies)
{
    shared_ptr<DayListUser> user(new DayListUser());
    if (!bodies.count("account") || !bodies.count("cipher"))
        return 0;
    // 验证账号密码长度是否符合要求
    int len = bodies["account"].size();
    if (len != 10 || (bodies["cipher"].size() < 8 || bodies["cipher"].size() > 20))
        return 0;
    int start = 0;
    // 验证账号是否由数字组成
    while (start < len)
    {
        if (!(bodies["account"][start] >= '0' || bodies["account"][start] <= '9'))
            return 0;
        ++start;
    }
    start = 0;
    len = bodies["cipher"].size();
    // 验证密码是否有数字字母组成
    while (start < len)
    {
        if (!((bodies["account"][start] >= '0' || bodies["account"][start] <= '9')
             || (bodies["account"][start] >= 'a' || bodies["account"][start] <= 'z')
             || (bodies["account"][start] >= 'A' || bodies["account"][start] <= 'Z')))
            return 0;
        ++start;
    }
    return user -> registe(bodies["account"], bodies["cipher"]);
}

/**
 * 用户上传日程
 * @params:userId, startTime, endTime, isAlarm, advancedAlarmMintes, location, describtion, remarks
 */
int uploadScheduleItemAPI(map<string, string>& bodies)
{
    if (!bodies.count("userId") || !bodies.count("startTime") 
        || !bodies.count("endTime") || !bodies.count("describtion"))
        return -1;
    if (bodies.count("isAlarm") && !bodies.count("advancedAlarmMintes"))
        return -1;
    /**
     * to do : 正则匹配判断 startTime 和 endTime 是否符合格式要求
     */
    shared_ptr<DayListUser> user(new DayListUser());
    return user -> uploadScheduleItem(bodies);
}

/**
 * 获取用户日程，每次10条
 * @params:userId, page
 */
int getUserItem(map<string, string>& urlData, string& items, char& more)
{
    if (!urlData.count("userId") || !urlData.count("page"))
        return -1;
    shared_ptr<DayListUser> user(new DayListUser());
    return user -> getUserItem(urlData, items, more);
}

/**
 * 获取用户信息
 */
int getUserInformation(map<string, string>& urlData, string& userInformation)
{
    if (!urlData.count("userId"))
        return -1;
    shared_ptr<DayListUser> user(new DayListUser());
    string userId = urlData["userId"];
    return user -> getUserInformation(userId, userInformation);
}

/**
 * 修改用户信息
 */
int modifyUserInformation(map<string, string>& bodies)
{
    if (!bodies.count("userId"))
        return -1;
    if (bodies.size() < 2)
        return -1;
    shared_ptr<DayListUser> user(new DayListUser());
    return user -> modifyUserInformation(bodies);
}
