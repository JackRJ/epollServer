/*
 * @Author: your name
 * @Date: 2020-05-18 21:49:09
 * @LastEditTime: 2020-06-17 17:32:00
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Ed
 * @FilePath: /try/src/API/DayListUser.h
 */ 
#pragma once
#include "../mysql/DayListUser.h"
#include <map>
#include <string>

int loginAPI(std::map<std::string, std::string>& bodies);
int registeAPI(std::map<std::string, std::string>& bodies);
