#include"Log.h"
#include<stdio.h>
#include<stdarg.h>
#include<iostream>

Log::Log()
    :m_isBlock(false)
{

}

Log::~Log()
{

}

bool Log::init(const char* logfile)
{
    return true;
}

void Log::setBlock(bool block)
{
    m_mutex.lock();
    m_isBlock = block;
    m_mutex.unlock();
}

void Log::operator() (const char* format, ...)
{
    static char text[1024];
    va_list ap;
    va_start(ap, format);
    vsnprintf(text, sizeof(text), format, ap);
    va_end(ap);

    m_mutex.lock();
    if (!m_isBlock)
    {
        std::cout << text << std::endl;
    }
    m_mutex.unlock();

    if (m_logfile)
    {
        m_logfile << text << std::endl;
    }
}