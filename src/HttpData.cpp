/*
 * @Author: your name
 * @Date: 2020-03-17 21:44:09
 * @LastEditTime: 2020-05-19 17:20:08
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
{ /*test*/ close(fd_); }

void HttpData::startup()
{
    handleRead();
    // size_t i = 0, j = 0;
    // int numchars;
    // char buf[1024];
    // char method[255];
    // char url[255];
    // // 读http 请求的第一行数据（request line），把请求方法存进 method 中
    // numchars = get_line(buf, sizeof(buf));
    // printf("buf: \n%s", buf);
    // // attention
    // while (!ISspace(buf[j]) && (i < sizeof(method) - 1))
    // {
    //     method[i] = buf[j];
    //     ++i; ++j;
    // }
    // method[i] = '\0';
    // //如果请求的方法不是 GET 或 POST 任意一个的话就直接发送 response 告诉客户端没实现该方法
    // if (strcasecmp(method, "GET") && strcasecmp(method, "POST"))
    // {
    //     unimplement();
    //     return;
    // }

    // // 如果是 POST 方法
    // if (strcasecmp(method, "POST") == 0)
    //     method_ = METHOD_POST;
    // else if (strcasecmp(method, "GET") == 0)
    //     method_ = METHOD_GET;
    // i = 0;
    // //跳过所有的空白字符(空格)
    // while (ISspace(buf[j]) && (j < sizeof(buf)))
    //     ++j;
    // // 然后把 URL 读出来放到 url 数组中
    // while (!ISspace(buf[j]) && (i < sizeof(url) - 1) && (j < sizeof(buf)))
    // {
    //     url[i] = buf[j];
    //     ++i; ++j;
    // }
    // url[i] = '\0';
    // url_ = url;
    // // 如果这个请求是一个 GET 方法的话
    // if (strcasecmp(method, "GET") == 0)
    // {
    //     // 去遍历这个 url，跳过字符 ？前面的所有字符，如果遍历完毕也没找到字符 ？则退出循环
    //     auto pos = url_.find('?');
    //     if (pos != url_.npos)
    //         // 如果是 ？ 的话，证明这个请求需要调用 cgi，将参数存入header_
    //         loadHeaders(pos);
    // }
}

void HttpData::handleRead()
{
    bool zero = false;
    int readNum = readn(fd_, inBuffer_, zero);
    do
    {
        if (readNum < 0)
        {
            error_ = true;
            perror("read");
            bad_request();
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
                    perror("parse url");
                    inBuffer_.clear();
                    error_ = true;
                    bad_request();
                    break;
                } else 
                    state_ = STATE_PARSE_HEADERS;
            }
        }
        if (state_ == STATE_PARSE_HEADERS)
        {
            HeaderState flag = parseHeader();
            /*printf("header nums: %i\n", headers_.size());
            for (auto h : headers_)
            {
                printf("%s : %s\n", h.first.c_str(), h.second.c_str());
            }*/
            if (flag == PARSE_HEADER_AGAIN)
                break;
            else if (flag == PARSE_HEADER_ERROR)
            {
                perror("parse header");
                error_ = true;
                bad_request();
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

int HttpData::parseBody()
{
    bool error = 0;
    std::string& str = inBuffer_;
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

void HttpData::loadHeaders(int beginPos)
{
    std::string::iterator startpos = url_.begin() + beginPos;
    do
    {
        ++startpos;
        auto midpos = std::find(startpos, url_.end(), '=');
        auto endpos = std::find(startpos, url_.end(), '&');
        headers_[std::string(startpos, midpos)] = std::string(++midpos, endpos);
        startpos = endpos;
    } while(startpos != url_.end());
}

AnalysisState HttpData::analysisRequest()
{
    if (method_ == METHOD_POST)
    {
        std::string header;
        header += "HTTP/1.1 200 OK\r\n";
        if (url_ == "login")
        {
            this -> parseBody();
            printf("bodies:%i\n", bodies.size());
            for (auto h : bodies)
            {
                printf("%s : %s\n", h.first.c_str(), h.second.c_str());
            }
            if (login(bodies) == 1)
            {
                outBuffer_ = "HTTP/1.1 200 OK\r\nContent-type: text/plain\r\n\r\nHello World";
                return ANALYSIS_SUCCESS;
            } else 
            {
                printf("login error\n");
            }
            bad_request();
            return ANALYSIS_ERROR;
        } else if (url_ == "register")
        {
            this -> parseBody();
            if (!bodies.count("account") || !bodies.count("cipher"))
                return ANALYSIS_ERROR;
            std::string account = bodies["account"];
            std::string cipher = bodies["cipher"];
            outBuffer_ = "HTTP/1.1 200 OK\r\nContent-type: text/plain\r\n\r\nHello World";
            if (registe(account, cipher) == 1)
                return ANALYSIS_SUCCESS;
            return ANALYSIS_ERROR;
        }
    } else if (method_ == METHOD_GET || method_ == METHOD_HEAD)
    {
        std::string header;
        header += "HTTP/1.1 200 OK\r\n";

        // test
        if (url_ == "hello")
        {
            printf("fileName: %s\n", url_.c_str());
            outBuffer_ = "HTTP/1.1 200 OK\r\nContent-type: text/plain\r\n\r\nHello World";
            return ANALYSIS_SUCCESS;
        }
    }
    return ANALYSIS_ERROR;
}

int HttpData::get_line(char* buf, int size)
{
    int i = 0;
    char c = '\0';
    int n;
    while ((i < size - 1) && (c != '\n'))
    {
        // recv()包含于<sys/socket.h>,参读《TLPI》P1259, 
        // 读一个字节的数据存放在 c 中
        n = recv(fd_, &c, 1, 0);
        if (n > 0)
        {
            if (c == '\r')
            {
                n = recv(fd_, &c, 1, 0);
                if ((n > 0) && (c == '\n'))
                    c = '\n';
                    // recv(fd_, &c, 1, 0);
                else
                    c = '\n';
            }
            buf[i] = c;
            ++i;
        } else
            c = '\n';
    }
    buf[i] = '\0';
    return i;
}

void HttpData::bad_request()
{
    char buf[1024];
    sprintf(buf, "HTTP/1.0 400 BAD REQUEST\r\n");
    send(fd_, buf, sizeof(buf), 0);
    sprintf(buf, "Content-type: text/html\r\n");
    send(fd_, buf, sizeof(buf), 0);
    sprintf(buf, "\r\n");
    send(fd_, buf, sizeof(buf), 0);
    sprintf(buf, "<P>Your browser sent a bad request, ");
    send(fd_, buf, sizeof(buf), 0);
    sprintf(buf, "such as a POST without a Content-Length.\r\n");
    send(fd_, buf, sizeof(buf), 0);
}

void HttpData::not_found()
{
    char buf[1024];
    sprintf(buf, "HTTP/1.0 404 NOT FOUND\r\n");
    send(fd_, buf, strlen(buf), 0);
    sprintf(buf, SERVER_STRING);
    send(fd_, buf, strlen(buf), 0);
    sprintf(buf, "Content-Type: text/html\r\n");
    send(fd_, buf, strlen(buf), 0);
    sprintf(buf, "\r\n");
    send(fd_, buf, strlen(buf), 0);
    sprintf(buf, "<HTML><TITLE>Not Found</TITLE>\r\n");
    send(fd_, buf, strlen(buf), 0);
    sprintf(buf, "<BODY><P>The server could not fulfill\r\n");
    send(fd_, buf, strlen(buf), 0);
    sprintf(buf, "your request because the resource specified\r\n");
    send(fd_, buf, strlen(buf), 0);
    sprintf(buf, "is unavailable or nonexistent.\r\n");
    send(fd_, buf, strlen(buf), 0);
    sprintf(buf, "</BODY></HTML>\r\n");
    send(fd_, buf, strlen(buf), 0);
}

void HttpData::unimplement()
{
    char buf[1024];
    sprintf(buf, "HTTP/1.0 501 Method Not Implemented\r\n");
    send(fd_, buf, strlen(buf), 0);
    sprintf(buf, SERVER_STRING);
    send(fd_, buf, strlen(buf), 0);
    sprintf(buf, "Content-Type: text/html\r\n");
    send(fd_, buf, strlen(buf), 0);
    sprintf(buf, "\r\n");
    send(fd_, buf, strlen(buf), 0);
    sprintf(buf, "<HTML><HEAD><TITLE>Method Not Implemented\r\n");
    send(fd_, buf, strlen(buf), 0);
    sprintf(buf, "</TITLE></HEAD>\r\n");
    send(fd_, buf, strlen(buf), 0);
    sprintf(buf, "<BODY><P>HTTP request method not supported.\r\n");
    send(fd_, buf, strlen(buf), 0);
    sprintf(buf, "</BODY></HTML>\r\n");
    send(fd_, buf, strlen(buf), 0);
}