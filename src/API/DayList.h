/*
 * @Author: your name
 * @Date: 2020-05-18 21:49:09
 * @LastEditTime: 2020-06-28 10:05:27
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Ed
 * @FilePath: /try/src/API/DayListUser.h
 */ 
#pragma once
#include "../mysql/DayListUser.h"
#include <time.h>
#include <map>
#include <string>

using namespace std;

class DayListAPI
{
private:
    shared_ptr<DayListUser> user;

    int checkCooie(const int& userId, const string& cookie);
public:
    DayListAPI();
    ~DayListAPI();

   /**
    * 用户登陆
    */
    int loginAPI(map<string, string>& headers_, map<string, string>& bodies, int& userId, string& header); 
};



/**
 * 用户注册
 */
int registeAPI(map<string, string>& headers_, map<string, string>& bodies, string& header, int& userId);

/**
 * 用户上传日程
 */
int uploadScheduleItemAPI(map<string, string>& headers_, map<string, string>& bodies);

/**
 * 获取用户日程，一次10条
 */
int getUserItem(map<string, string>& headers_, map<string, string>& urlData, string& items, char& more);


/**
 * 获取用户信息
 */
int getUserInformation(map<string, string>& headers_, map<string, string>& urlData, string& userInformation);

/**
 * 修改用户信息
 */
int modifyUserInformation(map<string, string>& headers_, map<string, string>& bodies);