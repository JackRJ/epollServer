/*
 * @Author: your name
 * @Date: 2020-07-05 11:25:56
 * @LastEditTime: 2020-07-05 11:27:41
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /try/src/base/CountDownLatch.cpp
 */ 
#include "CountDownLatch.h"

CountDownLatch::CountDownLatch(int count):
    mutex_(),
    condition_(mutex_),
    count_(count)
{ }

void CountDownLatch::wait()
{
    MutexLockGuard lock(mutex_);
    while (count_ > 0)
        condition_.wait();
}

void CountDownLatch::countDown()
{
    MutexLockGuard lock(mutex_);
    --count_;
    if (count_ == 0)
        condition_.notifyAll();
}