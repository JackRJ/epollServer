/*
 * @Author: your name
 * @Date: 2020-07-05 10:53:12
 * @LastEditTime: 2020-07-05 16:07:18
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /try/src/base/AsyncLogging.h
 */ 
#pragma once
#include "nonCopyable.h"
#include "CountDownLatch.h"
#include "LogStream.h"
#include "Thread.h"
#include <string>
#include <thread>
#include <vector>

class AsyncLogging: nonCopyable
{
public:
    AsyncLogging(const std::string basename, int flushInterval = 2);
    ~AsyncLogging()
    {
        if (running_)
            stop();
    }

    void append(const char* logline, int len);

    void start()
    {
        running_ = true;
        thread_.start();
        latch_.wait();
    }

    void stop()
    {
        running_ = false;
        cond_.notify();
        latch_.wait();
    }

private:
    bool running_;
    void threadFunc();
    typedef FixedBuffer<kLargeBuffer> Buffer;
    typedef std::vector<std::shared_ptr<Buffer>> BufferVector;
    typedef std::shared_ptr<Buffer> BufferPtr;
    const int flushInterval_;
    std::string basename_;
    Thread thread_;
    MutexLock mutex_;
    Condition cond_;
    CountDownLatch latch_;

    BufferPtr currentBuffer_;
    BufferPtr nextBuffer_;
    BufferVector buffers_;
};

