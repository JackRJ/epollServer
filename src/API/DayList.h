/*
 * @Author: your name
 * @Date: 2020-05-18 21:49:09
 * @LastEditTime: 2020-06-21 17:06:32
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
int loginAPI(std::map<std::string, std::string>& bodies);

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

