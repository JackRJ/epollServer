/*
 * @Author: your name
 * @Date: 2020-03-17 21:44:09
 * @LastEditTime: 2020-06-27 10:43:53
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /try/src/HttpData.cpp
 */
#include "HttpData.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <algorithm>
#include <sys/socket.h>
#include <netinet/in.h>

std::map<std::string, APIpath> HttpData::hash_ = 
    {
        std::make_pair("daylist/login", daylist_login),
        std::make_pair("daylist/register", daylist_register),
        std::make_pair("daylist/uploadScheduleItem", daylist_uploadScheduleItem),
        std::make_pair("daylist/getUserItems", daylist_getUserItems),
        std::make_pair("daylist/getUserInformation", daylist_getUserInformation),
        std::make_pair("daylist/modifyUserInformation", daylist_modifyUserInformation)
    };

HttpData::HttpData(int fd):
    fd_(fd),
    error_(false),
    HTTPVersion_(HTTP_11),
    method_(METHOD_GET),
    state_(STATE_PARSE_URI), 
    hState_(H_START),
    nowReadPos_(0)
{}

HttpData::~HttpData()
{
    // test http 1.0 
    // if (HTTPVersion_ == 1 || error_)
        close(fd_); 
}

void HttpData::startup()
{
    // start reading http data
    handleRead();
}

void HttpData::handleRead()
{
    bool zero = false;
    int readNum = readn(fd_, inBuffer_, zero);
    printf("read : %s\n", inBuffer_.c_str());
    do
    {
        if (readNum < 0)
        {
            error_ = true;
            perror("read error");
            handleError(400, "BAD REQUEST");
            break;
        } else if (zero)
        {
            // 有请求出现但是读不到数据，可能是Request Aborted，或者来自网络的数据没有达到等原因
            // 最可能是对端已经关闭了，统一按照对端已经关闭处理
            if (readNum == 0)
                break;
        }
        if (state_ == STATE_PARSE_URI)
        {
            if (!error_)
            {
                URIState flag = parseLine();
                if (flag == PARSE_URI_AGAIN)
                    break;
                else if (flag == PARSE_URI_ERROR)
                {
                    perror("parse url error");
                    inBuffer_.clear();
                    error_ = true;
                    handleError(400, "BAD REQUEST");
                    break;
                } else 
                    state_ = STATE_PARSE_HEADERS;
            }
        }
        if (state_ == STATE_PARSE_HEADERS)
        {
            HeaderState flag = parseHeader();
            if (flag == PARSE_HEADER_AGAIN)
                break;
            else if (flag == PARSE_HEADER_ERROR)
            {
                perror("parse header error");
                error_ = true;
                handleError(400, "BAD REQUEST");
                break;
            }
            if(method_ == METHOD_POST)
            {
                // POST方法准备
                state_ = STATE_RECV_BODY;
            }
            else 
            {
                state_ = STATE_ANALYSIS;
            }
        }
        if (state_ == STATE_RECV_BODY)
        {
            state_ = STATE_ANALYSIS;
        }
        if (state_ == STATE_ANALYSIS)
        {
            AnalysisState flag = this -> analysisRequest();
            if (flag == ANALYSIS_SUCCESS)
            {
                state_ = STATE_FINISH;
                break;
            } else 
            {
                error_ = true;
                break;
            }
        }
    } while (false);
    if (!error_)
    {
        if (outBuffer_.size() > 0)
        {
            handleWrite();
        }
    } else 
    {
        handleError(400, "BAD REQUEST");
    }
}

void HttpData::handleWrite()
{
    if (!error_)
    {
        if (writen(fd_, outBuffer_) < 0)
        {
            perror("writen");
            error_ = 1;
        }
    }
}

int HttpData::parseUrlData(int pos)
{
    std::string data = url_.substr(pos);
    int start = 0;
    int end = data.size();
    while (start < end)
    {
        std::string key;
        std::string val;
        int index = start;
        while (index < end && data[index] != '=')
            ++index;
        key = std::string(data.begin() + start, data.begin() + index);
        ++index;
        start = index;
        while (index < end && data[index] != '&')
            ++index;
        val = std::string(data.begin() + start, data.begin() + index);
        start = ++index;
        urlData[key] = val;
        printf("%s, %s\n", key.c_str(), val.c_str());
    }
}

URIState HttpData::parseLine()
{
    // solve method url and httpversion
    std::string &str = inBuffer_;
    std::string cop = str;
    // 读到完整的请求行再开始解析请求
    size_t pos = str.find('\r', nowReadPos_);
    if (pos < 0)
        return PARSE_URI_AGAIN;
    // 去掉请求行所占的空间，节省空间
    std::string request_line = str.substr(0, pos);
    if (str.size() > pos + 1)
        str = str.substr(pos + 1);
    else 
        str.clear();
    // Method
    int posGet = request_line.find("GET");
    int posPost = request_line.find("POST");
    int posHead = request_line.find("HEAD");
    if (posGet >= 0)
    {
        pos = posGet;
        method_ = METHOD_GET;
    }
    else if (posPost >= 0)
    {
        pos = posPost;
        method_ = METHOD_POST;
    }
    else if (posHead >= 0)
    {
        pos = posHead;
        method_ = METHOD_HEAD;
    }
    else
    {
        // method not found
        return PARSE_URI_ERROR;
    }
    // filename
    pos = request_line.find("/", pos);
    if (pos < 0)
    {
        url_ = "index.html";
        HTTPVersion_ = HTTP_11;
        return PARSE_URI_SUCCESS;
    } else 
    {
        size_t _pos = request_line.find(' ', pos);
        if (_pos < 0)
            return PARSE_URI_ERROR;
        else
        {
            if (_pos - pos > 1)
            {
                url_ = request_line.substr(pos + 1, _pos - pos - 1);
                size_t __pos = url_.find('?');
                // 有参数需要处理
                if (__pos != url_.npos)
                {
                    printf("%s\n", url_.substr(__pos + 1).c_str());
                    this -> parseUrlData(__pos + 1);
                }
                // params to do 
                if (__pos >= 0)
                    url_ = url_.substr(0, __pos);
            } else 
                url_ = "index.html";
        }
        pos = _pos;
    }
    // HTTP 版本号
    pos = request_line.find("/", pos);
    if (pos < 0)
        return PARSE_URI_ERROR;
    else
    {
        if (request_line.size() - pos <= 3)
            return PARSE_URI_ERROR;
        else 
        {
            std::string ver = request_line.substr(pos + 1, 3);
            if (ver == "1.0")
                HTTPVersion_ = HTTP_10;
            else if (ver == "1.1")
                HTTPVersion_ = HTTP_11;
            else
                return PARSE_URI_ERROR;
        }
    }
    return PARSE_URI_SUCCESS;
}

HeaderState HttpData::parseHeader()
{
    std::string &str = inBuffer_;
    int key_start = -1, key_end = -1, value_start = -1, value_end = -1;
    int now_read_line_begin = 0;
    bool notFinish = true;
    size_t i = 0;
    for (; i < str.size() && notFinish; ++i)
    {
        switch(hState_)
        {
            case H_START:
            {
                if (str[i] == '\n' || str[i] == '\r')
                    break;
                hState_ = H_KEY;
                key_start = i;
                now_read_line_begin = i;
                break;
            }
            case H_KEY:
            {
                if (str[i] == ':')
                {
                    key_end = i;
                    if (key_end - key_start <= 0)
                        return PARSE_HEADER_ERROR;
                    hState_ = H_COLON;
                } else if (str[i] == '\n' || str[i] == '\r')
                    return PARSE_HEADER_ERROR;
                break;
            }
            case H_COLON:
            {
                if (str[i] == ' ')
                    hState_ = H_SPACES_AFTER_COLON;
                else 
                    return PARSE_HEADER_ERROR;
                break;
            }
            case H_SPACES_AFTER_COLON:
            {
                hState_ = H_VALUE;
                value_start = i;
                break;
            }
            case H_VALUE:
            {
                if (str[i] == '\r')
                {
                    hState_ = H_CR;
                    value_end = i;
                    if (value_end - value_start <= 0)
                        return PARSE_HEADER_ERROR;
                } else if (i - value_start > 255)
                    return PARSE_HEADER_ERROR;
                break;
            }
            case H_CR:
            {
                if (str[i] == '\n')
                {
                    hState_ = H_LF;
                    std::string key(str.begin() + key_start, str.begin() + key_end);
                    std::string value(str.begin() + value_start, str.begin() + value_end);
                    headers_[key] = value;
                    now_read_line_begin = i;
                } else 
                    return PARSE_HEADER_ERROR;
                break;
            }
            case H_LF:
            {
                if (str[i] == '\r')
                    hState_ = H_END_CR;
                else 
                {
                    key_start = i;
                    hState_ = H_KEY;
                }
                break;
            }
            case H_END_CR:
            {
                if (str[i] == '\n')
                    hState_ = H_END_LF;
                else 
                    return PARSE_HEADER_ERROR;
                break;
            }
            case H_END_LF:
            {
                notFinish = false;
                key_start = i;
                now_read_line_begin = i;
                break;
            }
        }
    }
    if (hState_ == H_END_LF)
    {
        str = str.substr(i - 1);
        return PARSE_HEADER_SUCCESS;
    }
    str = str.substr(now_read_line_begin);
    return PARSE_HEADER_AGAIN;
}

int HttpData::parseBody(bool isDecode)
{
    if (isDecode)
    {
        // 对中文的url解码
        std::shared_ptr<UrlTranslation> translation(new UrlTranslation());
        std::shared_ptr<char> tmp(new char[inBuffer_.size() + 1]);
        translation -> urlDecode(inBuffer_.data(), tmp.get());
        inBuffer_ = std::string(tmp.get());
    }
    bool error = 0;
    std::string& str = inBuffer_;
    printf("inbuff : %s\n", str.c_str());
    int key_start = 0, key_end = 0;
    int val_start = 0;
    int i = 0;
    for (; i < str.size(); ++i)
    {
        if (str[i] == '=')
        {
            key_end = i;
            val_start = i + 1;
        } else if (str[i] == '&')
        {
            std::string key = std::string(str.begin() + key_start, str.begin() + key_end);
            std::string val = std::string(str.begin() + val_start, str.begin() + i);
            bodies[key] = val;
            key_start = i + 1;
        }
    }
    std::string key = std::string(str.begin() + key_start, str.begin() + key_end);
    std::string val = std::string(str.begin() + val_start, str.begin() + i);
    bodies[key] = val;
    if (error)
        return -1;
}

AnalysisState HttpData::analysisRequest()
{
    if (method_ == METHOD_POST)
    {
        std::string header;
        header += "HTTP/1.1 200 OK\r\nContent-type: application/json\r\n";
        switch (hash_[url_])
        {
            /**
             * 登陆
             */
            case daylist_login : 
            {
                this -> parseBody(0);
                int UserId = 0;
                int res = loginAPI(headers_, bodies, UserId, header);
                header += "\r\n";
                if (res == 1)
                    outBuffer_ = header + "{\"result\":\"1\",\"msg\":\"login success\",\"userId\":"
                        + std::to_string(UserId) +"}";
                else if (res == -1)
                    outBuffer_ = header + "{\"result\":\"0\",\"msg\":\"wrong cipher\"}";
                else 
                    outBuffer_ = header + "{\"result\":\"0\",\"msg\":\"error, try again\"}";
                return ANALYSIS_SUCCESS;
                break;
            }
            /**
             * 注册
             */
            case daylist_register : 
            {
                this -> parseBody(0);
                int userId = 0;
                int rst = registeAPI(headers_, bodies, header, userId);
                header += "\r\n";
                if (rst == 1)
                    outBuffer_ = header + "{\"result\":\"1\",\"msg\":\"login success\",\"userId\":"
                        + std::to_string(userId) +"}";
                else if (rst == -1) 
                    outBuffer_ = header + "{\"result\":\"0\",\"msg\":\"account exited\"}";
                else 
                    outBuffer_ = header + "{\"result\":\"0\",\"msg\":\"try again, maybe params error\"}";
                return ANALYSIS_SUCCESS;
                break;
            }
            /**
             * 上传日程item
             */
            case daylist_uploadScheduleItem:
            {
                // 对中文的url解码
                // 之后再进行解析请求体
                this -> parseBody(true);
                int rst = uploadScheduleItemAPI(headers_, bodies);
                header += "\r\n";
                if (rst == 1)
                    outBuffer_ = header + "{\"result\":\"1\",\"msg\":\"upload success\"}";
                else if (rst == -1)
                    outBuffer_ = header + "{\"result\":\"0\",\"msg\":\"params error\"}";
                else 
                    outBuffer_ = header + "{\"result\":\"0\",\"msg\":\"try again\"}";
                return ANALYSIS_SUCCESS;
                break;
            }
            /**
             * 修改用户信息
             */
            case daylist_modifyUserInformation:
            {
                // 对中文的url解码
                // 之后再进行解析请求体
                header += "\r\n";
                this -> parseBody(true);
                if (bodies.empty())
                    outBuffer_ = header + "{\"result\":\"0\",\"msg\":\"params error\"}";
                else 
                {
                    int res = modifyUserInformation(headers_, bodies);
                    if (res == -1)
                        outBuffer_ = header + "{\"result\":\"0\",\"msg\":\"params error\"}";
                    else if (res == 0)
                        outBuffer_ = header + "{\"result\":\"0\",\"msg\":\"try again\"}";
                    else if (res == 1)
                        outBuffer_ = header + "{\"result\":\"1\",\"msg\":\"success\"}";
                    else if (res == -2)
                        outBuffer_ = header + "{\"result\":\"0\",\"msg\":\"account not exicted\"}";
                    else 
                        break;
                }
                return ANALYSIS_SUCCESS;
            }
            default: return ANALYSIS_ERROR;
        }
    } else if (method_ == METHOD_GET || method_ == METHOD_HEAD)
    {
        std::string header;
        header += "HTTP/1.1 200 OK\r\nContent-type: application/json\r\n\r\n";

        switch(hash_[url_])
        {
            /**
             * 获取某个用户的所有日程,每次获取10条
             */
            case daylist_getUserItems:
            {
                std::string items;
                char more;
                int res = getUserItem(headers_, urlData, items, more);
                if (res == -1)
                    outBuffer_ = header + "{\"result\":\"0\",\"msg\":\"wrong params\"}";
                else if (res == 0)
                    outBuffer_ = header + "{\"result\":\"0\",\"msg\":\"try again\"}";
                else if (res == 1)
                {
                    outBuffer_ = header + "{\"result\":\"1\",\"msg\":\"success\",\"more\":" + more
                        + ",\"scheduleItems\":[" + items + "]}";
                } else 
                    break;
                return ANALYSIS_SUCCESS;
            }
            /**
             * 获取用户信息
             */
            case daylist_getUserInformation:
            {
                std::string userInformation;
                int res = getUserInformation(headers_, urlData, userInformation);
                if (res == -1)
                    outBuffer_ = header + "{\"result\":\"0\",\"msg\":\"wrong params\"}";
                else if (res == 0)
                    outBuffer_ = header + "{\"result\":\"0\",\"msg\":\"try again\"}";
                else if (res == 1)
                    outBuffer_ = header + "{\"result\":\"1\",\"msg\":\"success\",\"userInformation\":"
                    + userInformation + "}";
                else 
                    break;
                return ANALYSIS_SUCCESS;
            }
            default: break;
        }
        // test
        if (url_ == "hello")
        {
            printf("fileName: %s\n", url_.c_str());
            std::string str = "{\"result\" : 1, \"msg\" : success}\r\n\r\n";
            outBuffer_ = "HTTP/1.1 200 OK\r\nContent-type: application/json\r\nset-cookie: KLBRSID=5430ad6ccb1a51f38ac194049bce5dfe|1593066470|1593064328; Path=/\r\n\r\n" + str;
            return ANALYSIS_SUCCESS;
        }
    }
    return ANALYSIS_ERROR;
}

void HttpData::handleError(int err_num, std::string short_msg)
{
    std::string body_buff, header_buff;
    char send_buff[4096];
    body_buff += "<html><title>哎~出错了</title>";
    body_buff += "<body bgcolor=\"ffffff\">";
    body_buff += std::to_string(err_num) + short_msg;
    body_buff += "<hr><em> RJ's Web Server</em>\n</body></html>";

    header_buff += "HTTP/1.1 " + std::to_string(err_num) + short_msg + "\r\n";
    header_buff += "Content-Type: text/html\r\n";
    header_buff += "Connection: Close\r\n";
    header_buff += "Content-Length: " + std::to_string(body_buff.size()) + "\r\n";
    header_buff += "Server: LinYa's Web Server\r\n";;
    header_buff += "\r\n";

    // 错误处理不考虑writen不完的情况
    sprintf(send_buff, "%s", header_buff.c_str());
    writen(this -> fd_, send_buff, strlen(send_buff));
    sprintf(send_buff, "%s", body_buff.c_str());
    writen(this -> fd_, send_buff, strlen(send_buff));
}
