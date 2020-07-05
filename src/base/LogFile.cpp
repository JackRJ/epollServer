/*
 * @Author: your name
 * @Date: 2020-06-24 18:04:22
 * @LastEditTime: 2020-07-05 16:06:40
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /try/src/base/LogFile.cpp
 */ 
#include "LogFile.h"

using namespace std;

LogFile::LogFile(const std::string& basename, int flushEveryN):
    basename_(basename),
    flushEveryN_(flushEveryN),
    mutex_(new MutexLock),
    count_(0)
{
    file_.reset(new AppendFile(basename));
}

LogFile::~LogFile()
{ }

void LogFile::append(const char* logline, int len)
{
    MutexLockGuard guard(*mutex_);
    append_unlocked(logline, len);
}

void LogFile::flush()
{
    MutexLockGuard guard(*mutex_);
    file_ -> flush();
}

void LogFile::append_unlocked(const char* logline, int len)
{
    file_ -> append(logline, len);
    ++count_;
    if (count_ >= flushEveryN_)
    {
        count_ = 0;
        file_ -> flush();
    }
}