/*
 * @Author: your name
 * @Date: 2020-07-05 10:35:29
 * @LastEditTime: 2020-07-05 10:48:02
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /try/src/base/LogStream.cpp
 */ 
#include "LogStream.h"
#include <algorithm>

const char digits[] = "9876543210123456789";
const char* zero = digits + 9;

// From muduo
template<typename T> 
size_t convert(char buf[], T value)
{
    T i = value;
    char *p = buf;

    do
    {
        int lsd = static_cast<int>(i % 10);
        i /= 10;
        *p++ = zero[lsd];
    } while (i != 0);

    if (value < 0)
    {
        *p++ = '-';
    }
    *p = '\0';
    std::reverse(buf, p);

    return p - buf;
}


template<typename T>
void LogStream::formatInteger(T v)
{
    // buffer容不下kMaxNumericSize个字符的话会被直接丢弃
    if (buffer_.avail() >= kMaxNumericSize)
    {
        size_t len = convert(buffer_.current(), v);
        buffer_.add(len);
    }
}

LogStream& LogStream::operator<<(short s)
{
    *this << static_cast<int>(s);
    return *this;
}

LogStream& LogStream::operator<<(unsigned short s)
{
    *this << static_cast<unsigned int>(s);
    return *this;
}

LogStream& LogStream::operator<<(int i)
{
    formatInteger(i);
    return *this;
}

LogStream& LogStream::operator<<(unsigned int i)
{
    formatInteger(i);
    return *this;
}

LogStream& LogStream::operator<<(long l)
{
    formatInteger(l);
    return *this;
}

LogStream& LogStream::operator<<(unsigned long l)
{
    formatInteger(l);
    return *this;
}

LogStream& LogStream::operator<<(long long l)
{
    formatInteger(l);
    return *this;
}

LogStream& LogStream::operator<<(unsigned long long l)
{
    formatInteger(l);
    return *this;
}

LogStream& LogStream::operator<<(double v)
{
    if (buffer_.avail() >= kMaxNumericSize)
    {
        int len = snprintf(buffer_.current(), kMaxNumericSize, "%.12g", v);
        buffer_.add(len);
    }
    return *this;
}

LogStream& LogStream::operator<<(long double v)
{
    if (buffer_.avail() >= kMaxNumericSize)
    {
        int len = snprintf(buffer_.current(), kMaxNumericSize, "%.12Lg", v);
        buffer_.add(len);
    }
    return *this;
}