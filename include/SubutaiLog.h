#ifndef __SUBUTAI_LOG_H__
#define __SUBUTAI_LOG_H__

#include <streambuf>
#include <iosfwd>
#include <ios>
#include <string>
#include <ctime>
#include <istream>

namespace SubutaiLauncher
{
    enum LogLevel
    {
        LL_FATAL = 1,
        LL_ERROR,
        LL_WARNING,
        LL_INFO,
        LL_DEBUG
    };

    template <typename ch, typename tr>
        class BasicBuffer : public std::basic_streambuf<ch, tr>
    {
        protected:
            typedef std::basic_streambuf<ch, tr> Base;
            typedef std::basic_ios<ch, tr> IOS;
            typedef ch char_type;
            typedef tr char_traits;
            typedef typename Base::int_type int_type;
            typedef typename Base::pos_type pos_type;
            typedef typename Base::off_type off_type;
            typedef typename IOS::openmode openmode;

        public:
            BasicBuffer():
                _pb(char_traits::eof()),
                _ispb(false)
        {
            this->setg(0, 0, 0);
            this->setp(0, 0);
        }

            ~BasicBuffer()
            {
            }

            virtual int_type overflow(int_type c)
            {
                if (c != char_traits::eof())
                    return writeToDevice(char_traits::to_char_type(c));
                else
                    return c;
            }

            virtual int_type underflow()
            {
                if (_ispb)
                {
                    return _pb;
                }
                else
                {
                    int_type c = readFromDevice();
                    if (c != char_traits::eof())
                    {
                        _ispb = true;
                        _pb = c;
                        return c;
                    }
                }
            }

            virtual int_type uflow()
            {
                if (_ispb)
                {
                    _ispb = false;
                    return _pb;
                }
                else
                {
                    int_type c = readFromDevice();
                    if (c != char_traits::eof())
                    {
                        _pb = c;
                    }
                    return c;
                }
            }

            virtual int_type pbackfail(int_type c)
            {
                if (_ispb)
                {
                    return char_traits::eof();
                } 
                else
                {
                    _ispb = true;
                    _pb = c;
                    return c;
                }
            }

            virtual std::streamsize xsgetn(char_type* p, std::streamsize count)
            {
                std::streamsize copied = 0;
                while (count > 0)
                {
                    int_type c = uflow();
                    if (c == char_traits::eof()) break;
                    *p++ = char_traits::to_char_type(c);
                    ++copied;
                    --count;
                }
                return copied;
            }

        protected:
            static int_type charToInt(char_type c)
            {
                return char_traits::to_int_type(c);
            }

        private:
            virtual int_type readFromDevice()
            {
                return char_traits::eof();
            }

            virtual int_type writeToDevice(char_type)
            {
                return char_traits::eof();
            }

            int_type _pb;
            bool _ispb;

            BasicBuffer(const BasicBuffer&);
            BasicBuffer& operator = (const BasicBuffer&);
    };

#if defined(_MSC_VER)
    template class BasicBuffer<char, std::char_traits<char>>;
#endif

    typedef BasicBuffer<char, std::char_traits<char>> Buffer;

    class LogBuffer : public Buffer
    {
        public:
            LogBuffer(LogLevel lvl);
            ~LogBuffer();

            void setLevel(LogLevel lvl);

            LogLevel getLevel() const;
            
            void log(LogLevel level, const std::string& msg);

        private:
            int writeToDevice(char c);

        private:
            LogLevel _level;
            std::string _message;
    };

    class LogIOS : public virtual std::ios
    {
        public:
            LogIOS(LogLevel level);
            ~LogIOS();
            LogBuffer* rdbuf();

        protected:
            LogBuffer _buf;
    };


    class Logger : public LogIOS, public std::ostream
    {
        public:
            Logger(LogLevel level);
            ~Logger();
            Logger& level(LogLevel level);
            Logger& debug();
            Logger& info();
            Logger& warning();
            Logger& error();
            Logger& fatal();
        private:
            
    };

    class Log
    {
        public:
            static Log* instance();
            ~Log();
            Logger* logger();
        protected:
            Log();
            static Log* _instance;
        private:
            Logger* _logger;
    };
};

#endif
