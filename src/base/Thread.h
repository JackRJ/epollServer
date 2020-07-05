/*
 * @Author: your name
 * @Date: 2020-07-05 12:03:43
 * @LastEditTime: 2020-07-05 15:27:31
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /try/src/base/Thread.h
 */ 
#pragma once
#include "CountDownLatch.h"
#include "nonCopyable.h"
#include <functional>

class Thread: nonCopyable
{
public:
    typedef std::function<void ()> ThreadFunc;

    explicit Thread(const ThreadFunc&, const std::string& name = std::string());
    ~Thread();
    void start();
    int join();
    bool started() const { return started_; }
    pid_t tid() const { return tid_; }
    const std::string& name() const { return name_; }

private:
    void setDefaultName();
    bool started_;
    bool joined_;
    pthread_t pthreadId_;
    pid_t tid_;
    ThreadFunc func_;
    std::string name_;
    CountDownLatch latch_;
};

