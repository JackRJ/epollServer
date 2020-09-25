# epollServer
本项目基于面试需求为开发背景，开发了一个简易的 Http 服务器，使用了以下技术：

1、使用 epoll 对多个文件描述符进行读、写、连接的监听

2、基于循环队列开发工作队列

3、使用线程池提供工作线程从任务队列取任务处理

4、基于 epoll + 线程池的 Reactor 模式，实现并发服务器

5、使用 Mysql 数据库

6、使用 Redis 作为缓存

# Build
```cpp
make
```

# Usage
```cpp
./bin/main
```
