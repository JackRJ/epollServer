/*
 * @Author: your name
 * @Date: 2020-07-05 16:08:37
 * @LastEditTime: 2020-07-05 16:18:20
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /try/src/base/Logging.h
 */ 
#pragma once
#include "LogStream.h"
#include <pthread.h>

class AsyncLogging;

class Logger
{
private:
    class Impl
    {
    public:
        Impl(const char *fileName, int line);
        
        void formatTime();
        LogStream stream_;
        int line_;
        std::string basename_;
    };
    Impl impl_;
    static std::string logFileName_;
    
public:
    Logger(const char *fileName, int line);
    ~Logger();

    LogStream& stream() { return impl_.stream_; }
    static void setLogFileName(std::string fileName)
    {
        logFileName_ = fileName;
    }
    static std::string getLogFileName()
    {
        return logFileName_;
    }

};

#define LOG Logger(__FILE__, __LINE__).stream()
