#pragma once 

#include "Core/DateTime.h"
#include "Core/FileInfo.h"
#include <string>

#define LOG_TRACE(iLogLevel, iFormat, ...) \
do { \
    Realisim::Core::Logger &_logTraceLogger = Realisim::Core::Logger::getInstance(); \
    std::string format = std::string("[%s] ") + iFormat; \
    _logTraceLogger.log(iLogLevel, format.c_str(), __FUNCTION__, ##__VA_ARGS__); \
} while(0);

#define LOG_TRACE_ERROR(iLogLevel, iFormat, ...) \
do { \
    Realisim::Core::Logger &_logTraceLogger = Realisim::Core::Logger::getInstance(); \
    std::string format = std::string("-ERROR- [%s] ") + iFormat; \
    _logTraceLogger.log(iLogLevel, format.c_str(), __FUNCTION__, ##__VA_ARGS__); \
} while(0);


namespace Realisim
{
namespace Core
{
    //-----------------------------------------------------------------------------------------------------------------
    // This is a general purpose Logger implemented as a singleton.
    // 
    // IMPORTANT NOTES:
    //      To avoid issues with the lifetime of this object in your programs, it is adviced to instanciate (via getInstance())
    //      at the begining (first line) of your main.cpp. This will keep the instance of the Logger alive as long as possible,
    //      which might be useful if you try to log stuff while exiting the application.
    //
    class Logger
    {
    public:
        static Logger& getInstance();
        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;
        virtual ~Logger();

        enum LogLevelFlag{llNormal = 1 << 0, 
            llVerbose = 1 << 1, 
            llUserDefined };

        struct Config {
            Config();

            bool mLogToFile;
            bool mLogToStdOut;
            bool mAddTimestampToLogEntry;
            int mLogLevel;
            int mMaximumSizeInBytes; // a new files is created when max is reached.
            std::string mFilepath;
        };

        void flush();
        const Config& getConfig() const { return mConfig; }
        void log(const char* format, ...);
        void log(int iLogLevel, const char* format, ...);
        
        void setConfig(const Config& iConfig);

    protected:
        Logger();

        void closeLogFile();
        void logInternal(int iLogLevel, const char* format, va_list iArgs);
        void openLogFile(const std::string& iFilepath);
        void rotateLog();

        static Logger* mpInstance; //Singleton instance.

        Config mConfig;
        FILE* mpLogFile;
        Core::FileInfo mFileInfo;
    };

}
}