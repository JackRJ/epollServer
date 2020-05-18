/*
 * @Author: your name
 * @Date: 2020-03-16 20:08:57
 * @LastEditTime: 2020-05-18 09:46:58
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /try/Util.h
 */
#pragma once
#include "HttpData.h"
#include <memory>
#include <mysql/mysql.h>

struct Args
{
    int client;
    char* filename;
    Args(int _client):
        client(_client)
    { }
};

#define SERVER_STRING "Server: jdbhttpd/0.1.0\r\n"

int setSocketNonBlocking(int fd);
int socket_bind_listen(int port);
ssize_t readn(int fd, std::string &inBuffer, bool &zero);
ssize_t writen(int fd, std::string &sbuff);
void worker(std::shared_ptr<void> args);