/*
 * @Author: your name
 * @Date: 2020-05-18 09:48:55
 * @LastEditTime: 2020-06-19 16:22:19
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /try/src/mysql/test.cpp
 */ 
#include "DayListUser.h"
#include <stdio.h>
#include <map>
#include <string>

using namespace std;


int main()
{
    DayListUser daylist;
    map<string, string> mym;
    mym["userId"] = "2";
    mym["startTime"] = "2020-06-18 13:50:03";
    mym["endTime"] = "2020-06-20 13:50:03";
    mym["describtion"] = "摆地摊";
    mym["isAlarm"] = "1";
    mym["advancedAlarmMintes"] = "30";
    mym["remarks"] = "日程";
    mym["location"] = "揭阳";
    int res = daylist.uploadItem(mym);
    printf("%i\n", res);
}