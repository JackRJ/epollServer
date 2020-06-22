/*
 * @Author: your name
 * @Date: 2020-05-18 17:34:27
 * @LastEditTime: 2020-06-22 16:12:14
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
int DayListUser::login(std::map<std::string, std::string>& bodies, int& userId)
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
    std::string str = "select * from User where account = '" + bodies["account"] + "'";
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
    if (bodies["cipher"] == std::string(row[4]))
    {
        userId = (int)row[0];
        return 1;
    }
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

    int res = mysql_query(&conn, str.c_str());
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
 * 获取用户日程，每次10条
 * @params:userId, page
 */
int DayListUser::getUserItem(std::map<std::string, std::string>& urlData, std::string& items, char& more)
{
    // 获取参数
    std::string userId = urlData["userId"];
    std::string page = urlData["page"];
    std::string str = "select * from schedule where userId = '" + userId + "' order by startTime desc limit "
        + page + ",10;";
    // 查询数据库
    int res = mysql_query(&conn, str.c_str());
    if (res)
    {
        printf("mysql error\n");
        return 0;
    }
    // 获取结果
    result = mysql_store_result(&conn);
    // 获取item数目
    int rowcount = mysql_num_rows(result);
    if (rowcount == 10)
        more = '1';
    else 
        more = '0';
    int fieldcount = mysql_num_fields(result);
    MYSQL_ROW row = mysql_fetch_row(result);
    // 获取字段
    std::vector<std::string> vec(fieldcount);
    for(int i = 1; i < fieldcount; i++)
    {
        field = mysql_fetch_field_direct(result,i);
        vec[i] = field -> name;
    }
    // 将个行处理为 json 格式数据
    while (row != NULL)
    {
        items += "{";
        for(int i = 1; i < fieldcount; i++)
        {
            field = mysql_fetch_field_direct(result,i);
            items += ("\"" + vec[i] + "\":\"");
            items += row[i];
            items += "\",";
        }
        items.pop_back();
        items += "},";
        row = mysql_fetch_row(result);
    }
    items.pop_back();
    return 1;
}