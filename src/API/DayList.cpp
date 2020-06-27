/*
 * @Author: your name
 * @Date: 2020-05-18 21:47:43
 * @LastEditTime: 2020-06-27 10:03:55
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
int loginAPI(map<string, string>& headers_, map<string, string>& bodies, int& userId, string& header)
{
    // 验证cookie是否正确
    if (headers_.count("Cookie") && bodies.count("userId"))
    {
        shared_ptr<DayListUser> user(new DayListUser());
        int id = atoi(bodies["userId"].c_str());
        auto vec = user -> getCookie(id);
        // 此时数据库 cookie 有此项，并且请求报文有 cookie 
        if (!vec.empty())
        {
            // 获取数据库 cookie 时间
            string data = vec.at(3);
            // 获取当前时间
            time_t timep;
            struct tm *cur;
            struct tm last = { 0 };
            time(&timep); //获取从1970至今过了多少秒，存入time_t类型的timep
            cur = localtime(&timep);//用localtime将秒数转化为struct tm结构体
            last.tm_year = atoi(data.substr(0, 4).c_str()) - 1900;
            last.tm_mon = atoi(data.substr(5, 2).c_str()) - 1;
            last.tm_mday = atoi(data.substr(8, 2).c_str());
            last.tm_hour = atoi(data.substr(11, 2).c_str());
            last.tm_min = atoi(data.substr(14, 2).c_str());
            last.tm_sec = atoi(data.substr(17, 2).c_str());
            // 判断时间是否在三天之内
            double diff = difftime(mktime(cur), mktime(&last));//转换结构体为time_t,利用difftime,计算时间差  
            auto pos = headers_["Cookie"].find(";");
            if (headers_.count("Cookie") && diff < 86400 * 3 && vec[2] == headers_["Cookie"].substr(0, pos))
            {
                header += "Set-Cookie: cid = " + vec[2] + "; path = /daylist\r\n";
                userId = id;
                return 1;
            } else if (diff >= 86400 * 3)
            {
                user -> deleteCookie(id);
            }
                
        }
    }
    if (!bodies.count("account") || !bodies.count("cipher"))
        return -1;
    // 验证账号密码长度是否符合要求
    int len = bodies["account"].size();
    if (len != 10 || (bodies["cipher"].size() < 8 || bodies["cipher"].size() > 20))
        return -1;
    
    shared_ptr<DayListUser> user(new DayListUser());
    int result = user -> login(bodies, userId);
    if (result == 1)
    {
        string cid = to_string(rand() % (100000000));
        user -> updateCookie(userId, cid);
        header += "Set-Cookie: cid = " + cid + "; path = /daylist\r\n";
    }
    return result;
}

/**
 * 用户注册
 * @params:account, cipher
 */
int registeAPI(map<string, string>& headers_, map<string, string>& bodies, string& header, int& userId)
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
    int ans = user -> registe(bodies["account"], bodies["cipher"]);
    if (ans == 1)
    {
        int id = user -> getUserId(bodies["account"]);
        if (id == -1)
            return -1;
        string cid = to_string(rand() % (100000000));
        user -> updateCookie(id, cid);
        header += "Set-Cookie: cid = " + cid + "; path = /daylist\r\n";
        userId = user -> getUserId(bodies["account"]);
    }
    return ans;
}

/**
 * 用户上传日程
 * @params:userId, startTime, endTime, isAlarm, advancedAlarmMintes, location, describtion, remarks
 */
int uploadScheduleItemAPI(map<string, string>& headers_, map<string, string>& bodies)
{
    if (!headers_.count("Cookie"))
        return -1;
    if (!bodies.count("userId") || !bodies.count("startTime") 
        || !bodies.count("endTime") || !bodies.count("describtion"))
        return -1;
    if (bodies.count("isAlarm") && !bodies.count("advancedAlarmMintes"))
        return -1;

    shared_ptr<DayListUser> user(new DayListUser());
    // 验证 cookie 的正确性
    int userId = atoi(bodies["userId"].c_str());
    auto vec = user -> getCookie(userId);
    auto pos = headers_["Cookie"].find(";");
    if (vec[2] != headers_["Cookie"].substr(0, pos))
        return -1;
    /**
     * to do : 正则匹配判断 startTime 和 endTime 是否符合格式要求
     */
    
    return user -> uploadScheduleItem(bodies);
}

/**
 * 获取用户日程，每次10条
 * @params:userId, page
 */
int getUserItem(map<string, string>& headers_, map<string, string>& urlData, string& items, char& more)
{
    if (!headers_.count("Cookie"))
        return -1;
    if (!urlData.count("userId") || !urlData.count("page"))
        return -1;
    shared_ptr<DayListUser> user(new DayListUser());
    // 验证 cookie 的正确性
    int userId = atoi(urlData["userId"].c_str());
    auto vec = user -> getCookie(userId);
    auto pos = headers_["Cookie"].find(";");
    if (vec[2] != headers_["Cookie"].substr(0, pos))
        return -1;
    return user -> getUserItem(urlData, items, more);
}

/**
 * 获取用户信息
 */
int getUserInformation(map<string, string>& headers_, map<string, string>& urlData, string& userInformation)
{
    if (!headers_.count("Cookie"))
        return -1;
    if (!urlData.count("userId"))
        return -1;
    shared_ptr<DayListUser> user(new DayListUser());
    string userId = urlData["userId"];
    // 验证 cookie 的正确性
    auto vec = user -> getCookie(atoi(userId.c_str()));
    auto pos = headers_["Cookie"].find(";");
    if (vec[2] != headers_["Cookie"].substr(0, pos))
        return -1;
    return user -> getUserInformation(userId, userInformation);
}

/**
 * 修改用户信息
 */
int modifyUserInformation(map<string, string>& headers_, map<string, string>& bodies)
{
    if (!headers_.count("Cookie"))
        return -1;
    if (!bodies.count("userId"))
        return -1;
    if (bodies.size() < 2)
        return -1;
    shared_ptr<DayListUser> user(new DayListUser());
    // 验证 cookie 的正确性
    auto vec = user -> getCookie(atoi(bodies["userId"].c_str()));
    auto pos = headers_["Cookie"].find(";");
    if (vec[2] != headers_["Cookie"].substr(0, pos))
        return -1;
    return user -> modifyUserInformation(bodies);
}
