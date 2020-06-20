/*
 * @Author: your name
 * @Date: 2020-05-18 17:34:27
 * @LastEditTime: 2020-06-20 18:49:36
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /try/src/mysql/DayListUser.cpp
 */ 
#include "DayListUser.h"

DayListUser::DayListUser():
    connected(0)
{
    // 初始化数据库
    mysql_init(&conn);
    auto tmp = mysql_real_connect(&conn, "localhost", "jack", "lovezrj", "day_list_user",0,NULL, CLIENT_FOUND_ROWS);
    if (tmp)
        connected = 1;
    mysql_set_character_set(&conn, "utf8");
}

DayListUser::~DayListUser()
{
    // 关闭数据库
    if (connected)
        mysql_close(&conn);
}

/**
 * 登陆函数
 */
int DayListUser::login(std::map<std::string, std::string>& bodies)
{
    /*if (!connected)
        return -1;
    std::string str = "select cipher from User where account = '" + account + "'";
    return 0;*/
    if (!bodies.count("account") || !bodies.count("cipher"))
    {
        printf("mysql params error\n");
        return 0;
    }
    printf("mysql params success\n");
    std::string str = "select cipher from User where account = '" + bodies["account"] + "'";
    int res = mysql_query(&conn, str.c_str());
    if (res)
    {
        printf("mysql error\n");
        return 0;
    }
    result = mysql_store_result(&conn);
    MYSQL_ROW row = mysql_fetch_row(result);
    if (!row)
    {
        printf("!row\n");
        return 0;
    }
    if (bodies["cipher"] == std::string(row[0]))
        return 1;
    printf("not equ: %s %s\n", bodies["cipher"].c_str(), row[0]);
    return -1;
}

/**
 * 注册函数
 */
int DayListUser::registe(std::string& account, std::string& cipher)
{
    // to do 查重
    std::string str = "select * from User where account = '" + account + "'";
    int res = mysql_query(&conn, str.c_str());
    if (res)
    {
        printf("mysql error\n");
        return 0;
    }
    result = mysql_store_result(&conn);
    int rowcount = mysql_num_rows(result);
    if (rowcount != 0)
    {
        printf("account exited\n");
        return -1;
    }
    
    str = "insert into User (name, sex, age, cipher, account) values ('***', 0, 20, '" 
            + cipher + "','" + account + "');";
    res = mysql_query(&conn, str.c_str());
    if (res)
    {
        printf("mysql error\n");
        return 0;
    }
    my_ulonglong affected_row = mysql_affected_rows(&conn);
    printf("%d rows affected.\n", (int)affected_row);
    return 1;
}

/**
 * 上传日程
 */
int DayListUser::uploadScheduleItem(std::map<std::string, std::string>& item)
{
    if (!item.count("userId") || !item.count("startTime") || !item.count("endTime") || !item.count("describtion"))
        return -1;
    if (item.count("isAlarm") && !item.count("advancedAlarmMintes"))
        return -1;
    std::string isAlarm = item.count("isAlarm") ? "1" : "0";
    std::string& userId = item["userId"];
    std::string& startTime = item["startTime"];
    std::string& endTime = item["endTime"];
    std::string& describtion = item["describtion"];
    std::string advancedAlarmMintes = item.count("isAlarm") ? item["advancedAlarmMintes"] : "";
    std::string remarks = item.count("remarks") ? item["remarks"] : "";
    std::string location = item.count("location") ? item["location"] : "";

    std::string str = "insert into schedule (userId, isAlarm, advancedAlarmMintes, describtion, remarks, startTime, endTime, location) values ("
    + userId + ", " + isAlarm + ", " + advancedAlarmMintes + ", '"
    + describtion + "', '" + remarks + "', '" + startTime + "', '" + endTime + "', '" + location + "');";

    /*char* utfchar = new char[1024];
    if (gbk2utf8(utfchar, str.c_str(), 1024) == -1)
    {
        printf("gbk error.\n");
        delete[] utfchar;
        return 0;
    }*/
    printf("%s\n", str.c_str());
    int res = mysql_query(&conn, str.c_str());
    if (res)
    {
        printf("mysql error\n");
        //delete[] utfchar;
        return 0;
    }
    //delete[] utfchar;
    my_ulonglong affected_row = mysql_affected_rows(&conn);
    printf("%d rows affected.\n", (int)affected_row);
    return 1;
}

int DayListUser::gbk2utf8(char *utfStr,const char *srcStr,int maxUtfStrlen)
{
    if(NULL==srcStr)
    {
        printf("Bad Parameter\n");
        return -1;
    }
    printf("str not NULL\n");
    //首先先将gbk编码转换为unicode编码
    if(NULL==setlocale(LC_ALL,"zh_CN.gbk"))//设置转换为unicode前的码,当前为gbk编码
    {
        printf("Bad Parameter\n");
        return -1;
    }
    printf("gbk not null\n");
    int unicodeLen=mbstowcs(NULL,srcStr,0);//计算转换后的长度
    if(unicodeLen<=0)
    {
        printf("Can not Transfer!!!\n");
        return -1;
    }
    wchar_t *unicodeStr=(wchar_t *)calloc(sizeof(wchar_t),unicodeLen+1);
    mbstowcs(unicodeStr,srcStr, strlen(srcStr));//将gbk转换为unicode
    
    //将unicode编码转换为utf8编码
    if(NULL==setlocale(LC_ALL,"zh_CN.utf8"))//设置unicode转换后的码,当前为utf8
    {
        printf("Bad Parameter\n");
        return -1;
    }
    printf("utf8 not null\n");
    int utfLen=wcstombs(NULL,unicodeStr,0);//计算转换后的长度
    if(utfLen<=0)
    {
        printf("Can not Transfer!!!\n");
        return -1;
    }
    else if(utfLen>=maxUtfStrlen)//判断空间是否足够
    {
        printf("Dst Str memory not enough\n");
        return -1;
    }
    wcstombs(utfStr,unicodeStr,utfLen);
    utfStr[utfLen]=0;//添加结束符
    free(unicodeStr);
    return utfLen;
}