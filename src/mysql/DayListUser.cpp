/*
 * @Author: your name
 * @Date: 2020-05-18 17:34:27
 * @LastEditTime: 2020-06-27 10:43:31
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

int DayListUser::getUserId(const std::string& account)
{
    int userId = 0;
    std::string str = "select id from User where account = '" + account + "';";
    int res = mysql_query(&conn, str.c_str());
    if (res)
    {
        printf("mysql error\n");
        return -1;
    }
    result = mysql_store_result(&conn);
    int rowcount = mysql_num_rows(result);
    if (rowcount == 0)
        return -1;
    MYSQL_ROW row = mysql_fetch_row(result);
    char* tmp = row[0];
    int len = strlen(tmp);
    while (len--)
        userId = 10 * userId + (*(tmp++) - '0');
    return userId;
}

int DayListUser::deleteCookie(int userId)
{
    std::string str = "select * from cookie where userId = " + std::to_string(userId) + ";";
    int res = mysql_query(&conn, str.c_str());
    if (res)
    {
        printf("mysql error\n");
        return 0;
    }
    result = mysql_store_result(&conn);
    int rowcount = mysql_num_rows(result);
    if (rowcount == 0)
        return -1;
    str = "delete from cookie where userId = " + std::to_string(userId) + ";";
    res = mysql_query(&conn, str.c_str());
    if (res)
    {
        printf("mysql error\n");
        return 0;
    }
    return 1;
}

std::vector<std::string> DayListUser::getCookie(int userId)
{
    std::string str = "select * from cookie where userId = " + std::to_string(userId) + ";";
    int res = mysql_query(&conn, str.c_str());
    if (res)
    {
        printf("mysql error\n");
        return {};
    }
    result = mysql_store_result(&conn);
    int rowcount = mysql_num_rows(result);
    if (rowcount == 0)
        return {};
    MYSQL_ROW row = mysql_fetch_row(result);
    int fieldcount = mysql_num_fields(result);
    std::vector<std::string> ans(4);
    for (int i = 0; i < fieldcount; ++i)
    {
        printf("%s\n", row[i]);
        ans[i] = std::string(row[i]);
    }
    return ans;
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
    int id = 0;
    char* tmp = row[0];
    int count = strlen(tmp);
    while (count--)
        id = 10 * id + (*(tmp++) - '0');
    if (bodies["cipher"] == std::string(row[4]))
    {
        userId = id;
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
    int page = atoi(urlData["page"].c_str()) * 10;
    std::string str = "select * from schedule where userId = '" + userId + "' order by startTime desc limit "
        + std::to_string(page) + ",10;";
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
    if (rowcount == 0)
        return 1;
    else if (rowcount == 10)
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

/**
 * 获取用户信息
 */
int DayListUser::getUserInformation(const std::string& userId, std::string& userInformation)
{
    std::string str = "select * from User where id = " + userId + ";";
    // 查询数据库
    int res = mysql_query(&conn, str.c_str());
    if (res)
    {
        printf("mysql error\n");
        return 0;
    }
    // 获取结果
    result = mysql_store_result(&conn);
    int fieldcount = mysql_num_fields(result);
    MYSQL_ROW row = mysql_fetch_row(result);
    // 获取字段
    std::vector<std::string> vec(fieldcount);
    for(int i = 1; i < fieldcount; i++)
    {
        field = mysql_fetch_field_direct(result,i);
        vec[i] = field -> name;
    }
    userInformation += "{";
    for(int i = 1; i < fieldcount - 2; i++)
    {
        field = mysql_fetch_field_direct(result,i);
        userInformation += ("\"" + vec[i] + "\":\"");
        userInformation += row[i];
        userInformation += "\",";
    }
    userInformation.pop_back();
    return 1;
}

/**
 * 修改用户信息
 */
int DayListUser::modifyUserInformation(std::map<std::string, std::string>& bodies)
{
    const std::string userId = bodies["userId"];
    // 查找用户是否存在
    std::string str = "select * from User where id = " + userId + ";";
    int res = mysql_query(&conn, str.c_str());
    if (res)
    {
        printf("mysql error\n");
        return 0;
    }
    result = mysql_store_result(&conn);
    int rowcount = mysql_num_rows(result);
    if (rowcount == 0)
    {
        printf("account not excited\n");
        return -2;
    }

    // 查询数据库用户信息
    str =  "update User set ";
    if (bodies.count("name"))
        str += "name = '" + bodies["name"] + "', ";
    if (bodies.count("sex"))
        str += "sex = " + bodies["sex"] + ", ";
    if (bodies.count("age"))
        str += "age = '" + bodies["age"] + "', ";
    str.pop_back();
    str.pop_back();
    str += " where id = " + userId + ";";
    // 修改数据库
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
 * 更新cookie
 */
int DayListUser::updateCookie(int userId, const std::string& cid)
{

    std::string str = "select * from cookie where userId = " + std::to_string(userId) + ";";
    int res = mysql_query(&conn, str.c_str());
    if (res)
    {
        printf("mysql error\n");
        return {};
    }
    result = mysql_store_result(&conn);
    int rowcount = mysql_num_rows(result);
    // 没有记录
    if (rowcount == 0)
    {
        str = "insert into cookie (userId, cid) values (" 
            + std::to_string(userId) 
            + ", '" + cid + "');";
        // 更新数据库
        res = mysql_query(&conn, str.c_str());
        if (res)
        {
            printf("mysql error\n");
            return 0;
        }
        return 1;
    }
    // 有记录，更新即可
    str = "update cookie set cid = '" + cid + "' where userId = " + std::to_string(userId) + ";";
    // 更新数据库
    res = mysql_query(&conn, str.c_str());
    if (res)
    {
        printf("mysql error\n");
        return 0;
    }
    return 1;
}