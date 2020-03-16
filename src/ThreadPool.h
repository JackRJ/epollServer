/*
 * @Author: your name
 * @Date: 2020-03-07 05:47:55
 * @LastEditTime: 2020-03-16 20:58:50
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /webserver/ThreadPool.h
 */
// ensure this file has not been used
#pragma once
#include <vector>
#include <pthread.h>
#include <memory>
#include <functional>

const int THREADPOOL_INVALID = -1;
const int THREADPOOL_LOCK_FAILURE = -2;
const int THREADPOOL_QUEUE_FULL = -3;
const int THREADPOOL_SHUTDOWN = -4;
const int THREADPOOL_THREAD_FAILURE = -5;
const int THREADPOOL_GRACEFUL = -1;

const int MAX_THREADS = 1024;
const int MAX_QUEUE = 65535;

typedef enum
{
    immediate_shutdown = 1,
    graceful_shutdown = 2
} ShutDownOption;

struct ThreadPoolTask
{
    std::function<void(std::shared_ptr<void>)> fun;
    std::shared_ptr<void> args;
};


class ThreadPool
{
private:
    static pthread_mutex_t lock;
    static pthread_cond_t notify;

    static std::vector<pthread_t> threads;
    static std::vector<ThreadPoolTask> queue;
    static int thread_count;
    static int queue_size;
    static int head;
    // tail 指向尾结点的下一个结点
    static int tail;
    static int count;
    static int shutdown;
    static int started;
public:
    static int threadpool_create(int _thread_count, int queue_size);
    static int threadpool_add(std::shared_ptr<void> args, std::function<void(std::shared_ptr<void>)> fun);
    static int threadpool_destroy(ShutDownOption shutdown_option = graceful_shutdown);
    static int threadpool_free();
    static void *threadpool_thread(void *args);
    ThreadPool();
    ~ThreadPool();
};

