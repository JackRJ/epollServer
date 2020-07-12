/*
 * @Author: your name
 * @Date: 2020-03-16 20:05:44
 * @LastEditTime: 2020-07-12 23:20:45
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /try/main.cpp
 */
#include "Util.h"
#include "ThreadPool.h"
#include "base/Logging.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <memory>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>

#include "base/FileUtil.h"

#define SERV_PORT 8006
#define THREAD_NUMBERS 2
#define QUEUE_SIZE 1500

int epfd;
// 声明epoll_event结构体的变量,ev用于注册事件,数组用于回传要处理的事件
struct epoll_event ev, events[20];

int main()
{
    int i, maxi, listenfd, connfd, sockfd,nfds;
    struct sockaddr_in clientaddr;
    // struct sockaddr_in serveraddr;
    socklen_t clilen = sizeof(clientaddr);

    std::string logPath = "loger.txt";
    Logger::setLogFileName(logPath);
    // STL库在多线程上应用
    #ifndef _PTHREADS
        LOG << "_PTHREADS is not defined !";
    #endif

    
    // 创建线程池 RAII
    std::shared_ptr<ThreadPool> threadPool(new ThreadPool());
    threadPool -> threadpool_create(THREAD_NUMBERS, QUEUE_SIZE);

    // 生成用于处理accept的epoll专用的文件描述符，全局变量
    epfd = epoll_create(1);
    
    listenfd = socket_bind_listen(SERV_PORT);
    if(setSocketNonBlocking(listenfd) == -1)
        perror("setSocketNonBlocking");

    printf("server started at port: %i.\n", SERV_PORT);

    // 设置与要处理的事件相关的文件描述符
    ev.data.fd = listenfd;
    // 设置要处理的事件类型
    ev.events = EPOLLIN | EPOLLET;
    // 注册epoll事件
    epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev);

    maxi = 0;

    srand(time(0));
    while (1)
    {
        // 等待epoll事件的发生
        nfds = epoll_wait(epfd, events, 20, 500);

        //处理所发生的所有事件
        for (size_t i = 0; i < nfds; i++)
        {
            if(events[i].data.fd == listenfd)
            {
                // accept socket
                connfd = accept(listenfd, (sockaddr*)&clientaddr, &clilen);
                if (connfd == -1)
                {
                    perror("accept");
                    return -1;
                }
                setSocketNonBlocking(connfd);
                char *str = inet_ntoa(clientaddr.sin_addr);
                printf("accept address: %s\n", str);
                
                // 设置用于读操作的文件描述符
                ev.data.fd = connfd;
                // 设置用于注测的读操作事件
                ev.events = EPOLLIN | EPOLLET;
                // 注册ev
                epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &ev);
            } else if (events[i].events & EPOLLIN)
            {
                if ((sockfd = events[i].data.fd) < 0)
                    continue;
                
                // read socket
                // test
                std::shared_ptr<Args> args_(new Args(sockfd));
                std::shared_ptr<void> args = std::static_pointer_cast<void>(args_);
                threadPool -> threadpool_add(args, &worker);
            } else if (events[i].events & EPOLLOUT)
            {
                if ((sockfd = events[i].data.fd) < 0)
                    continue;
                // write socket
                // close(sockfd);
            }
        }
    }
    close(epfd);
    close(listenfd);
}