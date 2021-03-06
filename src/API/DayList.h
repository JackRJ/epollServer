/*
 * @Author: your name
 * @Date: 2020-05-18 21:49:09
 * @LastEditTime: 2020-08-02 16:54:40
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Ed
 * @FilePath: /try/src/API/DayListUser.h
 */ 
#pragma once
#include "../mysql/DayListUser.h"
#include "../redis/Redis.h"
#include <time.h>
#include <map>
#include <string>

using namespace std;

class DayListAPI
{
private:
    // cache redis
    shared_ptr<redis> myRedis;
    bool redis_connected;
    const char* redis_ip = "127.0.0.1";
    const short redis_port = 6379;
    // user
    shared_ptr<DayListUser> user;
    // http 请求头数据
    map<string, string>& headers_;
    // 返回数据
    string& outBuffer_;

    bool isSetCookie;
    string cookie_;

    int checkCooie(const int& userId, const string& cookie);
    int setOutBuffer(int res_num, string short_msg, string msg);
    string getCookie(const string& cookie_in_header);
    string getCurrentTime();
    
public:
    DayListAPI(map<string, string>& headers, string& outBuffer);
    ~DayListAPI();

   /**
    * 用户登陆
    */
    int loginAPI(map<string, string>& bodies);

    /**
     * 用户注册
     */
    int registeAPI(map<string, string>& bodies, int& userId);

    /**
     * 用户上传日程
     */
    int uploadScheduleItemAPI(map<string, string>& bodies);

    /**
     * 获取用户日程，一次10条
     */
    int getUserItem(map<string, string>& urlData);

    /**
     * 获取用户信息
     */
    int getUserInformation(map<string, string>& urlData);

    /**
     * 修改用户信息
     */
    int modifyUserInformation(map<string, string>& bodies);

    /**
     * 删除日程
     */
    int deleteScheduleItem(map<string, string>& bodies);
};


