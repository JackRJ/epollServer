/*
 * @Author: your name
 * @Date: 2020-06-21 10:51:21
 * @LastEditTime: 2020-06-21 11:24:12
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /try/src/base/UrlTranslation.cpp
 */ 
#include "UrlTranslation.h"

UrlTranslation::UrlTranslation()
{
    //
}

UrlTranslation::~UrlTranslation()
{
    //
}

char UrlTranslation::char_to_hex(char x)
{
    return (unsigned char)(x > 9 ? x + 55 : x + 48);
}

int UrlTranslation::is_alpha_number_char(char c)
{
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
        return 1;
    return 0;
}

/**
 * 测试中
 */
int UrlTranslation::encode(char* src, int src_len, char* dest, int dest_len)
{
    char c;
    int len = 0;
    while (len < (dest_len - 4) && *src)
    {
        c = (unsigned char)*src;
        if (*src == ' ')
            *dest++ = '+';
        else if (is_alpha_number_char(c) || strchr("=!~*'()", c))
            *dest++ = *src;
        else 
        {
            *dest = '%';
            *dest++ = char_to_hex( (char)(c >> 4) );
            *dest++ = char_to_hex( (char)(c % 16) );
        }
        ++src;
        ++len;
    }
    *dest = 0;
    return 1;
}

/**
 * url解码
 */
int UrlTranslation::decode(const char* encd, char* decd)
{
    int i, j = 0;
    char* cd = (char*)encd;
    char p[2];
    unsigned int num = 0;
    for (i = 0; i < strlen(cd); ++i)
    {
        /*if (cd[i] == '+')
            decd[j++] = 32;
        else */
        if (cd[i] != '%')
        {
            decd[j++] = cd[i];
            continue;
        } 
        // att
        memset(p, '\0', 2);
        p[0] = cd[++i];
        p[1] = cd[++i];
        p[0] = p[0] - 48 - ((p[0] >= 'A') ? 7 : 0) - ((p[0] >= 'a') ? 32 : 0); 
        p[1] = p[1] - 48 - ((p[1] >= 'A') ? 7 : 0) - ((p[1] >= 'a') ? 32 : 0); 
        decd[j++] = (char)(p[0] * 16 + p[1]);
    }
    decd[j] = '\0';
    return 1;
}

/**
 * @brief: 对外编码接口
 * @paramter: src: 
 */
int UrlTranslation::urlEncode(char* src, int src_len, char* dest, int dest_len)
{
    return 0;
}

/**
 * 
 */
int UrlTranslation::urlDecode(const char* encd, char* decd)
{
    return decode(encd, decd);
}
