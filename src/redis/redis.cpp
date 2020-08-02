/*
 * @Author: your name
 * @Date: 2020-08-02 15:03:13
 * @LastEditTime: 2020-08-02 15:28:36
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /redis/redis.cpp
 */ 
#include <stdio.h>
#include <iostream>
#include "redis.h"

int main()
{
    redis* r = new redis();
    bool res = r -> connect("127.0.0.1", 6379);
    std::cout << res << std::endl;
    if (!res)
    {
        printf("main error\n");
        return 0;
    }
    r -> set("name", "andy");
    printf("get name is %s\n", r -> get("name").c_str());
    delete r;
    return 0;
}