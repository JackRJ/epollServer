/*
 * @Author: your name
 * @Date: 2020-07-05 10:03:13
 * @LastEditTime: 2020-07-05 10:36:09
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /try/src/base/LogStream.h
 */ 
#pragma once
#include "nonCopyable.h"
#include <string.h>
#include <string>

const int kSmallBuffer = 4000;
const int kLargeBuffer = 4000 * 1000;

template <int SIZE>
class FixedBuffer: nonCopyable
{
private:
    char* cur;
    char data_[SIZE];
    const char* end() const  { return data_ + sizeof(data_); }
public:
    FixedBuffer():
        cur(data_)
    { }
    ~FixedBuffer()
    { }

    // 可用空间
    int avail() const { return static_cast<int>(end() - cur); }

    void append(const char* buf, size_t len)
    {
        if (avail() > static_cast<int>(len))
        {
            memcpy(cur, buf, len);
            cur += len;
        }
    }

    const char* data() const { return data_; }

    int length() const { return static_cast<int>(cur - data_); }

    char* current() { return cur; }

    void add(size_t len) { cur += len; }

    void reset() { cur = data_; }

    void bzero() { memset(data_, 0, sizeof(data_)); }
};


class LogStream: nonCopyable
{
public:
    typedef FixedBuffer<kSmallBuffer> Buffer;

    LogStream& operator<<(bool v)
    {
        buffer_.append(v ? "1" : "0", 1);
        return *this;
    }

    LogStream& operator<<(short);
    LogStream& operator<<(unsigned short);
    LogStream& operator<<(int);
    LogStream& operator<<(unsigned int);
    LogStream& operator<<(long);
    LogStream& operator<<(unsigned long);
    LogStream& operator<<(long long);
    LogStream& operator<<(unsigned long long);
    LogStream& operator<<(double);
    LogStream& operator<<(long double);

    LogStream& operator<<(const void*);

    LogStream& operator<<(float f)
    {
        *this << static_cast<double>(f);
        return *this;
    }

    LogStream& operator<<(char c)
    {
        buffer_.append(&c, 1);
        return *this;
    }

    LogStream& operator<<(const char* str)
    {
        if (str)
            buffer_.append(str, strlen(str));
        else 
            buffer_.append("(null)", 6);
        return *this;
    }

    LogStream& operator<<(const unsigned char* str)
    {
        *this << reinterpret_cast<const char*>(str);
        return *this;
    }

    LogStream& operator<<(const std::string& str)
    {
        buffer_.append(str.c_str(), str.size());
        return *this;
    }

    void append(const char* data, int len) { buffer_.append(data, len); }
    void resetBuffer() { buffer_.reset(); }
    const Buffer& buffer() const { return buffer_; }

private:
    void staticCheck();

    template<typename T>
    void formatInteger(T);

    Buffer buffer_;

    static const int kMaxNumericSize = 32;
};

