/*
 * @Author: your name
 * @Date: 2020-06-24 16:09:08
 * @LastEditTime: 2020-06-24 16:13:05
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /try/src/base/nonCopyable.h
 */ 
#pragma once

class nonCopyable
{
private:
    nonCopyable(const nonCopyable&);
    const nonCopyable operator=(const nonCopyable&);
protected:
    nonCopyable() { };
    ~nonCopyable() { };
};