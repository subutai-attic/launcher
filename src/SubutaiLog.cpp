#include "SubutaiLog.h"

namespace SubutaiLauncher
{

    Log* Log::_instance = NULL;

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
                _message.clear();
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

    Logger::Logger(LogLevel level) :
        LogIOS(level),
        std::ostream(&_buf)
    {

    }

    Logger::~Logger()
    {

    }

    Logger& Logger::level(LogLevel level)
    {
        _buf.setLevel(level);
        return *this;
    }

    Logger& Logger::debug()
    {
        return level(LL_DEBUG);
    }

    Logger& Logger::info()
    {
        return level(LL_INFO);
    }

    Logger& Logger::warning()
    {
        return level(LL_WARNING);
    }

    Logger& Logger::error()
    {
        return level(LL_ERROR);
    }

    Logger& Logger::fatal()
    {
        return level(LL_FATAL);
    }

    // ========================================================================

    Log::Log()
    {
        _logger = new Logger(LL_INFO);
    }

    Log::~Log()
    {
        delete _logger;
    }

    Log* Log::instance()
    {
        if (!_instance) _instance = new Log();
        return _instance;
    }

    Logger* Log::logger()
    {
        return _logger;
    }

};
