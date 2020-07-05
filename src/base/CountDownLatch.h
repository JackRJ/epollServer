/*
 * @Author: your name
 * @Date: 2020-07-05 11:04:37
 * @LastEditTime: 2020-07-05 15:46:09
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /try/src/base/CountDownLatch.h
 */ 
#pragma once
#include "Condition.h"
#include "nonCopyable.h"
#include "MutexLock.h"

// CountDownLatch的主要作用是确保Thread中传进去的func真的启动了以后
// 外层的start才返回
class CountDownLatch: nonCopyable
{
private:
    mutable MutexLock mutex_;
    Condition condition_;
    int count_;
public:
    explicit CountDownLatch(int count);

    void wait();
    void countDown();
};

