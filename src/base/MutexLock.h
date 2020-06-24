/*
 * @Author: your name
 * @Date: 2020-06-24 17:44:56
 * @LastEditTime: 2020-06-24 17:49:37
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /try/src/base/MutexLock.h
 */ 
#pragma once
#include "nonCopyable.h"
#include <pthread.h>

class MutexLock: nonCopyable
{
private:
    pthread_mutex_t mutex;
public:
    MutexLock()
    {
        pthread_mutex_init(&mutex, NULL);
    }
    ~MutexLock()
    {
        pthread_mutex_lock(&mutex);
        pthread_mutex_destroy(&mutex);
    }
    
    void lock()
    {
        pthread_mutex_lock(&mutex);
    }
    void unlock()
    {
        pthread_mutex_unlock(&mutex);
    }
    pthread_mutex_t* get()
    {
        return &mutex;
    }
};

class MutexLockGuard: nonCopyable
{
private:
    MutexLock &mutex;
public:
    explicit MutexLockGuard(MutexLock &_mutex):
        mutex(_mutex)
    {
        mutex.lock();
    }
    ~MutexLockGuard()
    {
        mutex.unlock();
    }
};

