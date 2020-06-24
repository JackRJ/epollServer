/*
 * @Author: your name
 * @Date: 2020-06-24 16:33:19
 * @LastEditTime: 2020-06-24 16:59:17
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /try/src/base/FileUtil.cpp
 */ 
#include "FileUtil.h"
#include <stdio.h>
#include <unistd.h>

using namespace std;

AppendFile::AppendFile(string fileName):
    fp_(fopen(fileName.c_str(), "ae"))
{
    // 用户提供缓冲区
    setbuffer(fp_, buffer_, sizeof(buffer_));
}

AppendFile::~AppendFile()
{
    fclose(fp_);
}

void AppendFile::append(const char *logline, const size_t len)
{
    size_t n = this -> write(logline, len);
    size_t remain = len - n;
    while (remain > 0)
    {
        size_t x = this -> write(logline + n, remain);
        if (x == 0)
        {
            int ferr = ferror(fp_);
            if (ferr)
                fprintf(stderr, "AppendFile::append() failed !\n");
            break;
        }
        n += x;
        remain = len - n;
    }
}

void AppendFile::flush()
{
    fflush(fp_);
}

size_t AppendFile::write(const char *logline, size_t len)
{
    return fwrite_unlocked(logline, 1, len, fp_);
}