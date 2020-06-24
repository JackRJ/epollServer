/*
 * @Author: your name
 * @Date: 2020-06-24 18:02:04
 * @LastEditTime: 2020-06-24 18:16:54
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /try/src/base/LogFile.h
 */ 
#include "MutexLock.h"
#include "FileUtil.h"
#include <memory>
#include <string>

class LogFile: nonCopyable
{
private:
    std::unique_ptr<MutexLock> mutex_;
    std::unique_ptr<AppendFile> file_;
    void append_unlocked(const char* logline, int len);

    const std::string basename_;
    const int flushEveryN_;

    int count_;
public:
    // 每append flushEveryN次，flush一下，会往文件写，只不过，文件也是带缓冲区的
    LogFile(const std::string& basename, int flushEveryN);
    ~LogFile();

    void append(const char* logline, int len);
    void flush();
    bool rollFile();
};
