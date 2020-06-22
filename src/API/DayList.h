/*
 * @Author: your name
 * @Date: 2020-05-18 21:49:09
 * @LastEditTime: 2020-06-22 17:37:17
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Ed
 * @FilePath: /try/src/API/DayListUser.h
 */ 
#pragma once
#include "../mysql/DayListUser.h"
#include <map>
#include <string>

/**
 * 用户登陆
 */
int loginAPI(std::map<std::string, std::string>& bodies, int& userId);

/**
 * 用户注册
 */
int registeAPI(std::map<std::string, std::string>& bodies);

/**
 * 用户上传日程
 */
int uploadScheduleItemAPI(std::map<std::string, std::string>& bodies);

/**
 * 获取用户日程，一次10条
 */
int getUserItem(std::map<std::string, std::string>& urlData, std::string& items, char& more);


/**
 * 获取用户信息
 */
int getUserInformation(std::map<std::string, std::string>& urlData, std::string& userInformation);

/**
 * 修改用户信息
 */
int modifyUserInformation(std::map<std::string, std::string>& bodies);