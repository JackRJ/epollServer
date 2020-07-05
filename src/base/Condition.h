/*
 * @Author: your name
 * @Date: 2020-07-05 10:56:34
 * @LastEditTime: 2020-07-05 11:03:44
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /try/src/base/Condition.h
 */ 
#pragma once
#include "nonCopyable.h"
#include "MutexLock.h"
#include <pthread.h>
#include <errno.h>
#include <time.h>

class Condition: nonCopyable
{
private:
    MutexLock& mutex;
    pthread_cond_t cond;
public:
    explicit Condition(MutexLock& mutex_):
        mutex(mutex_)
    {
        pthread_cond_init(&cond, NULL);
    }
    ~Condition() { pthread_cond_destroy(&cond); }

    void wait()
    {
        pthread_cond_wait(&cond, mutex.get());
    }

    void notify()
    {
        pthread_cond_signal(&cond);
    }

    void notifyAll()
    {
        pthread_cond_broadcast(&cond);
    }

    bool waitForSeconds(int seconds)
    {
        struct timespec abstime;
        clock_gettime(CLOCK_REALTIME, &abstime);
        abstime.tv_sec += static_cast<time_t>(seconds);
        return ETIMEDOUT == pthread_cond_timedwait(&cond, mutex.get(), &abstime);
    }
};
