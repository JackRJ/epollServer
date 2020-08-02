/*
 * @Author: your name
 * @Date: 2020-08-02 14:50:19
 * @LastEditTime: 2020-08-02 15:25:26
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /redis/redis.h
 */ 
#pragma once
#include <hiredis/hiredis.h>
#include <string>

class redis
{
private:
    redisContext* _connect;
    redisReply* _reply;
public:
    redis(){}
    ~redis()
    {
        this -> _connect = NULL;
        this -> _reply = NULL;
    }

    bool connect(std::string host, int port)
    {
        this -> _connect = redisConnect(host.c_str(), port);
        if(this->_connect != NULL && this->_connect->err)
        {
            printf("connect error : %s!\n", this -> _connect -> errstr);
            printf("error\n");
            return 0;
        }
        printf("success\n");
        return 1;
    }

    void set(std::string key, std::string value)
    {
        redisCommand(this -> _connect, "SET %s %s", key.c_str(), value.c_str());
    }

    std::string get(std::string key)
    {
        this -> _reply = (redisReply*)redisCommand(this->_connect, "GET %s", key.c_str());
        std::string str = this -> _reply -> str;
        freeReplyObject(this -> _reply);
        return str;
    }
};

