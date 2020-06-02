/*
 * @Author: your name
 * @Date: 2020-03-16 20:09:28
 * @LastEditTime: 2020-06-02 16:24:14
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /try/Util.cpp
 */
#include "Util.h"
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

const int MAX_BUFF = 4096;

int setSocketNonBlocking(int fd)
{
    int flag = fcntl(fd, F_GETFL, 0);
    if (flag == -1)
        return -1;

    flag |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flag) == -1)
        return -1;
    return 0;
}

int socket_bind_listen(int port)
{
    // check port 
    if (port < 0 || port > 65535)
        return -1;

    // create socket return fd
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd == -1)
    {
        perror("listen error!");
        return -1;
    }
    // 消除bind时"Address already in use"错误
    int optval = 1;
    if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
        return -1;
    
    // 设置服务器IP和Port，和监听描述副绑定
    struct sockaddr_in server_addr;
    bzero((char*)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons((unsigned short)port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("bind error");
        return -1;
    }

    // 开始监听，最大等待队列长为LISTENQ
    if (listen(listen_fd, 2048) == -1)
        return -1;
    // 无效监听描述符
    if (listen_fd == -1)
    {
        close(listen_fd);
        return -1;
    }
    return listen_fd;
}

ssize_t readn(int fd, std::string &inBuffer, bool &zero)
{
    ssize_t nread = 0;
    ssize_t readSum = 0;
    while (true)
    {
        char buff[MAX_BUFF];
        if ((nread = read(fd, buff, MAX_BUFF)) < 0)
        {
            if (errno == EINTR)
                continue;
            else if (errno == EAGAIN)
            {
                return readSum;
            }  
            else
            {
                perror("read error");
                return -1;
            }
        }
        else if (nread == 0)
        {
            zero = true;
            break;
        }
        readSum += nread;
        inBuffer += std::string(buff, buff + nread);
    }
    return readSum;
}

ssize_t writen(int fd, std::string &sbuff)
{
    size_t nleft = sbuff.size();
    ssize_t nwritten = 0;
    ssize_t writeSum = 0;
    const char *ptr = sbuff.c_str();
    while (nleft > 0)
    {
        if ((nwritten = write(fd, ptr, nleft)) <= 0)
        {
            if (nwritten < 0)
            {
                if (errno == EINTR)
                {
                    nwritten = 0;
                    continue;
                }
                else if (errno == EAGAIN)
                    break;
                else
                    return -1;
            }
        }
        writeSum += nwritten;
        nleft -= nwritten;
        ptr += nwritten;
    }
    if (writeSum == static_cast<int>(sbuff.size()))
        sbuff.clear();
    else
        sbuff = sbuff.substr(writeSum);
    return writeSum;
}

void worker(std::shared_ptr<void> args)
{
    std::shared_ptr<Args> args_ = std::static_pointer_cast<Args>(args);
    // malloc error?
    // std::shared_ptr<HttpData> httpData(new HttpData(args_ -> client));
    // httpData -> startup();
    HttpData httpData(args_ -> client);
    httpData.startup();
}

/*int login(std::map<std::string, std::string>& bodies)
{
    MYSQL conn;
    MYSQL_RES *result = NULL;
    MYSQL_FIELD *field = NULL;
    mysql_init(&conn);
    auto tmp = mysql_real_connect(&conn, "localhost", "jack", "lovezrj", "day_list_user",0,NULL, CLIENT_FOUND_ROWS);
    if (!bodies.count("account") || !bodies.count("cipher"))
    {
        printf("params error\n");
        return 0;
    }
    printf("params success\n");
    std::string str = "select cipher from User where account = '" + bodies["account"] + "'";
    int res = mysql_query(&conn, str.c_str());
    if (res)
    {
        printf("mysql error\n");
        return 0;
    }
    result = mysql_store_result(&conn);
    MYSQL_ROW row = mysql_fetch_row(result);
    if (!row)
    {
        printf("!row\n");
        return 0;
    }
    if (bodies["cipher"] == std::string(row[0]))
        return 1;
    printf("not equ: %s %s\n", bodies["cipher"].c_str(), row[0]);
    return 0;
    mysql_close(&conn);
}

int registe(std::string account, std::string cipher)
{
    MYSQL conn;
    MYSQL_RES *result = NULL;
    MYSQL_FIELD *field = NULL;
    mysql_init(&conn);
    auto tmp = mysql_real_connect(&conn, "localhost", "jack", "lovezrj", "day_list_user",0,NULL, CLIENT_FOUND_ROWS);
    // to do 查重
    std::string str = "select * from User where account = '" + account + "'";
    int res = mysql_query(&conn, str.c_str());
    if (res)
    {
        printf("mysql error\n");
        return 0;
    }
    result = mysql_store_result(&conn);
    int rowcount = mysql_num_rows(result);
    if (rowcount != 0)
    {
        printf("account exited\n");
        return 0;
    }
    
    str = "insert into User (name, sex, age, cipher, account) values ('***', 0, 20, '" 
            + cipher + "','" + account + "');";
    res = mysql_query(&conn, str.c_str());
    if (res)
    {
        printf("mysql error\n");
        return 0;
    }
    my_ulonglong affected_row = mysql_affected_rows(&conn);
    printf("%d rows affected.\n", (int)affected_row);
    return 1;
    mysql_close(&conn);
}*/