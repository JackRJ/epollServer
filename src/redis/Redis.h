/*
 * @Author: your name
 * @Date: 2020-08-02 14:50:19
 * @LastEditTime: 2020-08-02 16:23:21
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
    redis();
    ~redis()
    {
        this -> _connect = NULL;
        this -> _reply = NULL;
    }

    bool connect(std::string host, int port);

    void set_key_val(const std::string& key, const std::string& value);

    std::string get_key_val(const std::string& key);
};

