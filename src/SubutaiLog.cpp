#include "SubutaiLog.h"

namespace SubutaiLauncher
{

    LogBuffer::LogBuffer(LogLevel level) : _level(level)
    {

    }

    LogBuffer::~LogBuffer()
    {

    }

    void LogBuffer::setLevel(LogLevel level)
    {
        _level = level;
    }

    LogLevel LogBuffer::getLevel() const
    {
        return _level;
    }

    void LogBuffer::log(LogLevel level, const std::string& msg)
    {
        char* levels[] = {"", "FATAL", "ERROR", "WARNING", "INFO", "DEBUG", NULL};
        std::printf("[%s] %s\n", levels[level], msg.c_str());
    }

    int LogBuffer::writeToDevice(char c)
    {
        if (c == '\n' || c == '\r')
        {
            if (_message.find_first_not_of("\r\n") != std::string::npos)
            {
                log(_level, _message);
            }
        }
        else _message += c;
        return c;
    }

    // ========================================================================
    
    LogIOS::LogIOS(LogLevel level) : _buf(level)
    {
        init(&_buf);
    }

    LogIOS::~LogIOS()
    {

    }

    LogBuffer* LogIOS::rdbuf()
    {
        return &_buf;
    }
    
    // ========================================================================

    Log::Log(LogLevel level) :
        LogIOS(level),
        std::ostream(&_buf)
    {

    }

    Log::~Log()
    {

    }

    Log& Log::level(LogLevel level)
    {
        _buf.setLevel(level);
        return *this;
    }

    Log& Log::debug(const std::string& message)
    {
//        _buf.log(message);
        return level(LL_DEBUG);
    }

    Log& Log::debug()
    {
        return level(LL_DEBUG);
    }

    Log& Log::info(const std::string& message)
    {
//        _buf.log(message);
        return level(LL_INFO);
    }

    Log& Log::info()
    {
        return level(LL_INFO);
    }

    Log& Log::warning(const std::string& message)
    {
//        _buf.log(message);
        return level(LL_WARNING);
    }

    Log& Log::warning()
    {
        return level(LL_WARNING);
    }

    Log& Log::error(const std::string& message)
    {
//        _buf.log(message);
        return level(LL_FATAL);
    }

    Log& Log::error()
    {
        return level(LL_ERROR);
    }

    Log& Log::fatal(const std::string& message)
    {
//        _buf.log(message);
        return level(LL_FATAL);
    }

    Log& Log::fatal()
    {
        return level(LL_FATAL);
    }
};
