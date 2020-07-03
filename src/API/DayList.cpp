/*
 * @Author: your name
 * @Date: 2020-05-18 21:47:43
 * @LastEditTime: 2020-07-03 13:51:56
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /try/API/DayListUser.cpp
 */ 
#include "DayList.h"
#include <memory>

DayListAPI::DayListAPI(map<string, string>& headers, string& outBuffer):
    headers_(headers),
    outBuffer_(outBuffer),
    isSetCookie(0),
    user(new DayListUser())
{ }

DayListAPI::~DayListAPI()
{ }

int DayListAPI::checkCooie(const int& userId, const string& cookie)
{
    auto vec = user -> getCookie(userId);
    if (vec.empty())
        return -1;
    // 获取数据库 cookie 时间
    string data = vec.at(3);
    // 获取当前时间
    time_t timep;
    struct tm *cur;
    struct tm last = { 0 };
    time(&timep); //获取从1970至今过了多少秒，存入time_t类型的timep
    cur = localtime(&timep);//用localtime将秒数转化为struct tm结构体
    // 获取数据库 cookie 有效时间
    last.tm_year = atoi(data.substr(0, 4).c_str()) - 1900;
    last.tm_mon = atoi(data.substr(5, 2).c_str()) - 1;
    last.tm_mday = atoi(data.substr(8, 2).c_str());
    last.tm_hour = atoi(data.substr(11, 2).c_str());
    last.tm_min = atoi(data.substr(14, 2).c_str());
    last.tm_sec = atoi(data.substr(17, 2).c_str());
    // 转换结构体为time_t,利用difftime,计算时间差  
    // 判断时间是否在三天之内
    double diff = difftime(mktime(cur), mktime(&last));
    
    if (diff < 86400 * 3 && vec[2] == getCookie(cookie))
        return 1;
    else if (diff >= 86400 * 3 && vec[2] == getCookie(cookie))
        return 0;
    return -1;
}

string DayListAPI::getCookie(const string& cookie_in_header)
{
    // 获取 cookie 数据里面的 cid
    auto pos = cookie_in_header.find("cid");
    if (pos == cookie_in_header.npos)
        return "";
    int count = 0;
    pos += 4;
    while (pos + count < cookie_in_header.size() && cookie_in_header[pos + count] >= '0' 
        && cookie_in_header[pos + count] <= '9')
        ++count;
    return cookie_in_header.substr(pos, count);
}

int DayListAPI::setOutBuffer(int res_num, string short_msg, string msg)
{
    // header
    outBuffer_ = "HTTP/1.1 " + to_string(res_num) + " "
         + short_msg + "\r\nContent-type: application/json\r\n";
    if (isSetCookie)
    {

        outBuffer_ += "Set-Cookie: cid = " + cookie_ + "; path = /daylist\r\n";
        isSetCookie = 0;
    }
    outBuffer_ += "\r\n";

    // body
    outBuffer_ += msg;
    return 1;
}

/**
 * 用户登陆
 * @params:account, cipher
 */
int DayListAPI::loginAPI(map<string, string>& bodies)
{
    bool error = 0;
    int userId = 0;
    do
    {
        // 验证cookie是否正确
        if (headers_.count("Cookie") && bodies.count("userId"))
        {
            userId = atoi(bodies["userId"].c_str());
            int res = checkCooie(userId, headers_["Cookie"]);
            // 此时数据库 cookie 有此项，并且请求报文有 cookie 
            if (res == 1)
            {
                isSetCookie = 1;
                cookie_ = getCookie(headers_["Cookie"]);
                break;
            } else if (!(bodies.count("account") && bodies.count("cipher")))
            {
                //  expired
                setOutBuffer(401, "Unauthorized", "{\"error\":\"cookie expired!\"}");
                user -> deleteCookie(userId);
                return 0;
            }
        }
        if (!bodies.count("account") || !bodies.count("cipher"))
        {
            error = 1;
            break;
        }
        // 验证账号密码长度是否符合要求
        int accountLen = bodies["account"].size();
        int cipherLen = bodies["cipher"].size();
        if (accountLen != 10 || (cipherLen < 8 || cipherLen > 20))
        {
            error = 1;
            break;
        }
        int result = user -> login(bodies, userId);
        if (result == 1)
        {
            cookie_ = to_string(rand() % (100000000));
            user -> updateCookie(userId, cookie_);
            isSetCookie = 1;
        } else if (result == 0)
        {
            setOutBuffer(400, "INVALID REQUEST", "{\"error\":\"account not exicted!\"}");
            return 0;
        } else if (result == -1)
        {
            setOutBuffer(400, "INVALID REQUEST", "{\"error\":\"wrong cipher!\"}");
            return 0;
        }
    } while (false);
    if (error)
    {
        setOutBuffer(400, "INVALID REQUEST", "{\"error\":\"request must contain correct account and cipher!\"}");
        return 0;
    }
    setOutBuffer(200, "OK", "{\"status\":\"1\",\"msg\":\"login success\",\"userId\":"
                        + std::to_string(userId) +"}");
    return 1;
}

/**
 * 用户注册
 * @params:account, cipher
 */
int DayListAPI::registeAPI(map<string, string>& bodies, int& userId)
{
    bool error = 0;
    do
    {
        if (!bodies.count("account") || !bodies.count("cipher"))
        {
            error = 1;
            break;
        }
        // 验证账号密码长度是否符合要求
        int accountLen = bodies["account"].size();
        int cipherLen = bodies["cipher"].size();
        if (accountLen != 10 || (cipherLen < 8 || cipherLen > 20))
        {
            error = 1;
            break;
        }
        int start = 0;
        // 验证账号是否由数字组成
        while (start < accountLen)
        {
            if (!(bodies["account"][start] >= '0' || bodies["account"][start] <= '9'))
            {
                error = 1;
                break;
            }
            ++start;
        }
        start = 0;
        // 验证密码是否有数字字母组成
        while (!error && start < cipherLen)
        {
            if (!((bodies["account"][start] >= '0' || bodies["account"][start] <= '9')
                || (bodies["account"][start] >= 'a' || bodies["account"][start] <= 'z')
                || (bodies["account"][start] >= 'A' || bodies["account"][start] <= 'Z')))
            {
                error = 1;
                break;
            }
            ++start;
        }
    } while (false);
    
    if (error)
    {
        this -> setOutBuffer(400, "INVALID REQUEST", "{\"error\":\"request must contain correct account and cipher!\"}");
        return 0;
    }
    
    int ans = user -> registe(bodies["account"], bodies["cipher"]);
    if (ans == 1)
    {
        int id = user -> getUserId(bodies["account"]);
        if (id == -1)
        {
            this -> setOutBuffer(500, "INTERNAL SERVER ERROR", "{\"error\":\"mysql error, try again!\"}");
            return -1;
        }
        isSetCookie = 1;
        cookie_ = to_string(rand() % (100000000));
        user -> updateCookie(id, cookie_);
        userId = user -> getUserId(bodies["account"]);
    } else if (ans == -1)
    {
        setOutBuffer(400, "INVALID REQUEST", "{\"error\":\"account exited!\"}");
        return -1;
    } else if (ans == 0)
    {
        this -> setOutBuffer(500, "INTERNAL SERVER ERROR", "{\"error\":\"mysql error, try again!\"}");
        return 0;
    }
    setOutBuffer(200, "OK", "{\"status\":\"1\",\"msg\":\"registe success\",\"userId\":"
                        + std::to_string(userId) +"}");
    return ans;
}

/**
 * 用户上传日程
 * @params:userId, startTime, endTime, isAlarm, advancedAlarmMintes, location, describtion, remarks
 */
int DayListAPI::uploadScheduleItemAPI(map<string, string>& bodies)
{
    bool error = 0;
    do
    {
        if (!headers_.count("Cookie") || (!bodies.count("userId") || !bodies.count("startTime")
            || !bodies.count("endTime") || !bodies.count("describtion")
            || (bodies.count("isAlarm") && !bodies.count("advancedAlarmMintes"))))
        {
            error = 1;
            break;
        }
        // 验证 cookie 的正确性
        int userId = atoi(bodies["userId"].c_str());
        int res = checkCooie(userId, headers_["Cookie"]);
        if (res == 0)
        {
            setOutBuffer(401, "Unauthorized", "{\"error\":\"expired cookie, please login again!\"}");
            return 0;
        } else if (res != 1)
        {
            setOutBuffer(400, "INVALID REQUEST", "{\"error\":\"wrong cookie!\"}");
            return 0;
        }
    } while (false);
    if (error)
    {
        setOutBuffer(400, "INVALID REQUEST", "{\"error\":\"request must contain right params!\"}");
        return 0;
    }
    /**
     * to do : 正则匹配判断 startTime 和 endTime 是否符合格式要求
     */    
    int res = user -> uploadScheduleItem(bodies);
    if (res > 0)
    {
        setOutBuffer(200, "OK", "{\"status\":\"1\",\"msg\":\"upload success\",\"scheduleId\":" 
            + to_string(res) + "}");
    } else if (res == -1)
    {
        setOutBuffer(500, "INTERNAL SERVER ERROR", "{\"error\":\"mysql error, try again!\"}");
    }
    return res;
}

/**
 * 获取用户日程，每次10条
 * @params:userId, page
 */
int DayListAPI::getUserItem(map<string, string>& urlData)
{
    bool error = 0;
    do
    {
        if (!headers_.count("Cookie") || (!urlData.count("userId") || !urlData.count("page")))
        {
            error = 1;
            break;
        }
        // 验证 cookie 的正确性
        int userId = atoi(urlData["userId"].c_str());
        int res = checkCooie(userId, headers_["Cookie"]);
        if (res == 0)
        {
            setOutBuffer(401, "Unauthorized", "{\"error\":\"expired cookie, please login again!\"}");
            return 0;
        } else if (res != 1)
        {
            setOutBuffer(400, "INVALID REQUEST", "{\"error\":\"wrong cookie!\"}");
            return 0;
        }
    } while (false);
    
    if (error)
    {
        setOutBuffer(400, "INVALID REQUEST", "{\"error\":\"request must contain right params!\"}");
        return 0;
    }
    string items;
    char more;
    int res = user -> getUserItem(urlData, items, more);
    if (res == 0)
    {
        setOutBuffer(500, "INTERNAL SERVER ERROR", "{\"error\":\"mysql error, try again!\"}");
    }
    setOutBuffer(200, "OK", "{\"status\":\"1\",\"msg\":\"success\",\"more\":" + string(1, more) + ",\"scheduleItems\":[" + items + "]}");
    return res;
}

/**
 * 获取用户信息
 */
int DayListAPI::getUserInformation(map<string, string>& urlData)
{
    bool error = 0;
    do
    {
        if (!headers_.count("Cookie") || !urlData.count("userId"))
        {
            error = 1;
            break;
        }
        // 验证 cookie 的正确性
        int userId = atoi(urlData["userId"].c_str());
        int res = checkCooie(userId, headers_["Cookie"]);
        if (res == 0)
        {
            setOutBuffer(401, "Unauthorized", "{\"error\":\"expired cookie, please login again!\"}");
            return 0;
        } else if (res != 1)
        {
            setOutBuffer(400, "INVALID REQUEST", "{\"error\":\"wrong cookie!\"}");
            return 0;
        }
        string userInformation;
        res = user -> getUserInformation(to_string(userId), userInformation);
        if (res == 0)
        {
            setOutBuffer(500, "INTERNAL SERVER ERROR", "{\"error\":\"mysql error, try again!\"}");
            return 0;
        }
        setOutBuffer(200, "OK", "{\"status\":\"1\",\"msg\":\"success\",\"userInformation\":"
                    + userInformation + "}");
        
    } while (false);
    
    if (error)
    {
        setOutBuffer(400, "INVALID REQUEST", "{\"error\":\"request must contain right params!\"}");
        return 0;
    }
    return 1;
}

/**
 * 修改用户信息
 */
int DayListAPI::modifyUserInformation(map<string, string>& bodies)
{
    bool error = 0;
    do
    {
        if (!headers_.count("Cookie") || !bodies.count("userId") || bodies.size() < 2)
        {
            error = 1;
            break;
        }
        // 验证 cookie 的正确性
        int userId = atoi(bodies["userId"].c_str());
        int res = checkCooie(userId, headers_["Cookie"]);
        if (res == 0)
        {
            setOutBuffer(401, "Unauthorized", "{\"error\":\"expired cookie, please login again!\"}");
            return 0;
        } else if (res != 1)
        {
            setOutBuffer(400, "INVALID REQUEST", "{\"error\":\"wrong cookie!\"}");
            return 0;
        }
    } while (false);
    
    if (error)
    {
        setOutBuffer(400, "INVALID REQUEST", "{\"error\":\"request must contain right params!\"}");
        return 0;
    }
    int res = user -> modifyUserInformation(bodies);
    if (res == 0)
    {
        setOutBuffer(500, "INTERNAL SERVER ERROR", "{\"error\":\"mysql error, try again!\"}");
    } else if (res == -2)
    {
        setOutBuffer(400, "INVALID REQUEST", "{\"error\":\"account not exicted!\"}");
    }
    setOutBuffer(200, "OK", "{\"status\":\"1\",\"msg\":\"modify success\"}");
    return res;
}

/**
 * 删除日程
 */
int DayListAPI::deleteScheduleItem(map<string, string>& bodies)
{
    bool error = 0;
    do
    {
        if (!bodies.count("userId") || !bodies.count("scheduleId") || !headers_.count("Cookie"))
        {
            error = 1;
            break;
        }
        // 验证 cookie 的正确性
        int userId = atoi(bodies["userId"].c_str());
        int res = checkCooie(userId, headers_["Cookie"]);
        if (res == 0)
        {
            setOutBuffer(401, "Unauthorized", "{\"error\":\"expired cookie, please login again!\"}");
            return 0;
        } else if (res != 1)
        {
            setOutBuffer(400, "INVALID REQUEST", "{\"error\":\"wrong cookie!\"}");
            return 0;
        }
    } while (false);
    
    if (error)
    {
        setOutBuffer(400, "INVALID REQUEST", "{\"error\":\"request must contain right params!\"}");
        return 0;
    }
    int res = user -> deleteScheduleItem(bodies);
    if (res == 0)
    {
        setOutBuffer(500, "INTERNAL SERVER ERROR", "{\"error\":\"mysql error, try again!\"}");
    } else if (res == -1)
    {
        setOutBuffer(400, "INVALID REQUEST", "{\"error\":\"schedule not exicted!\"}");
    }
    setOutBuffer(200, "OK", "{\"status\":\"1\",\"msg\":\"delete success\"}");
    return res;
}