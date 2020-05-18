/*
 * @Author: your name
 * @Date: 2020-03-17 21:44:00
 * @LastEditTime: 2020-05-18 21:56:43
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /try/src/HttpData.h
 */
#pragma once
#include "Util.h"
#include "API/DayList.h"
#include <map>
#include <string>
#include <vector>

#define ISspace(x) isspace((int)(x))
#define SERVER_STRING "Server: jdbhttpd/0.1.0\r\n"

enum HttpMethod
{
    METHOD_POST = 1,
    METHOD_GET,
    METHOD_HEAD
};

enum HttpVersion
{
    HTTP_10 = 1,
    HTTP_11
};

enum URIState
{
    PARSE_URI_AGAIN = 1,
    PARSE_URI_ERROR,
    PARSE_URI_SUCCESS,
};

enum AnalysisState
{
    ANALYSIS_SUCCESS = 1,
    ANALYSIS_ERROR
};

enum ProcessState
{
    STATE_PARSE_URI = 1,
    STATE_PARSE_HEADERS,
    STATE_RECV_BODY,
    STATE_ANALYSIS,
    STATE_FINISH
};

enum HeaderState
{
    PARSE_HEADER_SUCCESS = 1,
    PARSE_HEADER_AGAIN,
    PARSE_HEADER_ERROR
};

enum ParseState
{
    H_START = 0,
    H_KEY,
    H_COLON,
    H_SPACES_AFTER_COLON,
    H_VALUE,
    H_CR,
    H_LF,
    H_END_CR,
    H_END_LF
};

class HttpData
{
private:
    int fd_;
    bool error_;
    HttpMethod method_;
    std::string url_;
    HttpVersion HTTPVersion_;
    std::map<std::string, std::string> headers_;
    std::map<std::string, std::string> bodies;
    std::string inBuffer_;
    std::string outBuffer_;
    int nowReadPos_;

    void bad_request();
    void not_found();
    void unimplement();
    void loadHeaders(int beginPos);
    int get_line(char* buf, int size);
    
    URIState parseLine();
    ProcessState state_;
    ParseState hState_;
    HeaderState parseHeader();
    AnalysisState analysisRequest();

    void handleRead();
    void handleWrite();
    int parseBody();
public:
    void startup();
    HttpData(int fd);
    ~HttpData();
};

