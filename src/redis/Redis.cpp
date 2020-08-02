/*
 * @Author: your name
 * @Date: 2020-08-02 15:03:13
 * @LastEditTime: 2020-08-02 16:39:42
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /redis/redis.cpp
 */ 
#include <stdio.h>
#include <iostream>
#include "Redis.h"


redis::redis():
    _connect(NULL),
    _reply(NULL)
{}

/**
 * connect to redis
 */
bool redis::connect(std::string host, int port)
{
    this -> _connect = redisConnect(host.c_str(), port);
    if(this->_connect != NULL && this->_connect->err)
    {
        printf("connect error : %s!\n", this -> _connect -> errstr);
        return 0;
    }
    printf("redis connect successfully!\n");
    return 1;
}

/**
 * set key val data into redis
 */
void redis::set_key_val(const std::string& key, const std::string& value)
{
    redisCommand(this -> _connect, "SET %s %s", key.c_str(), value.c_str());
}

/**
 * get key val data from redis
 */
std::string redis::get_key_val(const std::string& key)
{
    this -> _reply = (redisReply*)redisCommand(this->_connect, "GET %s", key.c_str());
    std::string str = this -> _reply -> str;
    freeReplyObject(this -> _reply);
    return str;
}
