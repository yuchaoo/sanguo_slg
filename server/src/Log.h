#ifndef _LOG_H_
#define _LOG_H_

#include "Ref.h"
#include<mutex>
#include<string>
#include<fstream>

class Log : public Ref
{
public:
    Log();
    ~Log();
    bool init(const char* logfile);
    void operator() (const char* format, ...);
    void setBlock(bool block);
private:
    std::mutex m_mutex;
    bool m_isBlock;
    std::ofstream m_logfile;
};

static Log g_log;

extern Log g_log;

#endif