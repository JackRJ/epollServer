/*
 * @Author: your name
 * @Date: 2020-06-21 09:32:14
 * @LastEditTime: 2020-06-21 11:04:55
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /try/src/base/UrlTranslation.h
 */ 
#pragma once 
#include <string.h>

class UrlTranslation
{
private:
    static char char_to_hex(char x);
    static int is_alpha_number_char(char c);

    int encode(char* src, int src_len, char* dest, int dest_len);
    int decode(const char* encd, char* decd);
public:
    int urlEncode(char* src, int src_len, char* dest, int dest_len);
    int urlDecode(const char* encd, char* decd);
    
    UrlTranslation();
    ~UrlTranslation();
};