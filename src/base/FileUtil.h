/*
 * @Author: your name
 * @Date: 2020-06-24 16:30:29
 * @LastEditTime: 2020-06-24 16:48:30
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /try/src/base/FileUtil.h
 */ 
#pragma once
#include "nonCopyable.h"
#include <string>

class AppendFile: nonCopyable
{
private:
    FILE* fp_;
    char buffer_[64 * 1024];
    size_t write(const char *logline, size_t len);
    
public:

    void flush();
    // append 会向文件写
    void append(const char *logline, const size_t len);

    explicit AppendFile(std::string fileName);
    ~AppendFile();
};

