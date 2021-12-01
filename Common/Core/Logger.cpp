
#include <cassert>
#include "Core/DateTime.h"
#include "Core/Logger.h"
#include "Core/StringUtilities.h"
#include <iostream>
#include <cstdarg>

#ifdef WIN32
#pragma warning(disable:4996)
#endif // WIN


using namespace Realisim;
    using namespace Core;
using namespace std;

// static initialization
Logger* Logger::mpInstance = nullptr;

//---------------------------------------------------------------------------------------------------------------------
Logger::Logger() :
    mpLogFile(nullptr)
{}

//---------------------------------------------------------------------------------------------------------------------
Logger::~Logger()
{
    closeLogFile();

    delete mpInstance;
    mpInstance = nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
void Logger::closeLogFile()
{
    if (mpLogFile)
    {
        fclose(mpLogFile);
        mpLogFile = nullptr;
        mFileInfo = FileInfo();
    }
}

//---------------------------------------------------------------------------------------------------------------------
Logger& Logger::getInstance()
{
    if (mpInstance == nullptr) {
        mpInstance = new Logger();
    }
    return *mpInstance;
}

//---------------------------------------------------------------------------------------------------------------------
void Logger::flush()
{
    if (mpLogFile)
        fflush(mpLogFile);
}

//---------------------------------------------------------------------------------------------------------------------
void Logger::log(const char* iFormat, ...)
{
    va_list args;
    va_start(args, iFormat);

    logInternal(llNormal, iFormat, args);
    va_end(args);
}

//---------------------------------------------------------------------------------------------------------------------
void Logger::log(int iLogLevel, const char* iFormat, ...)
{
    va_list args;
    va_start(args, iFormat);

    logInternal(iLogLevel, iFormat, args);
    va_end(args);
}

//---------------------------------------------------------------------------------------------------------------------
void Logger::logInternal(int iLogLevel, const char* iFormat, va_list iArgs)
{
    // early out.
    // do not log when logLevel if higher than config.
    //
    if ((iLogLevel & mConfig.mLogLevel) == 0) return;

    std::string format(iFormat);

    if (mConfig.mLogToStdOut) {
        if (mConfig.mAddTimestampToLogEntry)
            printf("%s - ", DateTime::currentDateTime().toString().c_str());
        vprintf(format.c_str(), iArgs);
        printf("\n");
    }

    if (mConfig.mLogToFile) {
        if (mConfig.mAddTimestampToLogEntry)
            fprintf(mpLogFile, "%s - ", DateTime::currentDateTime().toString().c_str());
        vfprintf(mpLogFile, format.c_str(), iArgs);
        fprintf(mpLogFile, "\n");
    }

    rotateLog();
}

//---------------------------------------------------------------------------------------------------------------------
void Logger::openLogFile(const std::string& iFilepath)
{
    mpLogFile = fopen(iFilepath.c_str(), "w");
    mFileInfo.setFile(iFilepath);
    mFileInfo.setCached(false);
    assert(mpLogFile);
}

//---------------------------------------------------------------------------------------------------------------------
void Logger::rotateLog()
{
    if (mFileInfo.getSize() >= mConfig.mMaximumSizeInBytes)
    {
        const std::string timestamp = DateTime::currentDateTime().toString();
        std::string newLogFilename = mFileInfo.getCompleteBaseName() + "_" + timestamp + "." + mFileInfo.getSuffix();

        // replace all : by _
        replaceAllOccurenceOf(&newLogFilename, ' ', "_");
        replaceAllOccurenceOf(&newLogFilename, ':', "_");

        string newLogFilePath = mFileInfo.getAbsolutePath() + "/" + newLogFilename;

        closeLogFile();

        openLogFile(newLogFilePath);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void Logger::setConfig(const Config& iConfig)
{
    if (mConfig.mLogToFile != iConfig.mLogToFile || 
        mConfig.mFilepath != iConfig.mFilepath)
    {
        if (mpLogFile) {
            closeLogFile();
        }
    }

    mConfig = iConfig;

    if (mConfig.mLogToFile && !mConfig.mFilepath.empty())
    {
        openLogFile(mConfig.mFilepath);
    }
}

//---------------------------------------------------------------------------------------------------------------------
Logger::Config::Config() :
    mLogToFile(false),
    mLogToStdOut(true),
    mAddTimestampToLogEntry(true),
    mLogLevel(Logger::llNormal),
    mMaximumSizeInBytes(256 * 1024 * 1024),
    mFilepath()
{}