#include "gtest/gtest.h"
#include "Core/Logger.h"

using namespace Realisim;
using namespace Core;

TEST(Logger, test)
{
    Logger& l = Logger::getInstance();
    Logger::Config c;
    c.mAddTimestampToLogEntry = false;
    c.mLogToFile = false;
    l.setConfig(c);

    l.log(Logger::llNormal, "Sans parametre...");
    l.log(Logger::llNormal, "%s", "patate");
    l.log(Logger::llNormal, "%s, %d", "patate", 4);

    // using the macro
    c.mAddTimestampToLogEntry = true;
    l.setConfig(c);
    LOG_TRACE(Logger::llNormal, "LOG_TRACE Sans parametre...");
    LOG_TRACE(Logger::llNormal, "LOG_TRACE %s", "patate");
    LOG_TRACE(Logger::llNormal, "LOG_TRACE %s, %d", "patate", 4);

    LOG_TRACE(Logger::llVerbose, "LOG_TRACE VERBOSE %s, %d", "patate", 4);

    /// log to file    
    //c.mLogToFile = true;
    //c.mFilepath = "./caca.txt";
    //l.setConfig(c);

    //l.log("%s", "un autre test dans un fichier");
    //LOG_TRACE(Logger::llNormal, "un chiffer en float: %.4f", 0.12345);
    //LOG_TRACE(Logger::llNormal, "LOG TRACE sans parametre");

    // log to multiple files
    //c.mMaximumSizeInBytes = 100;
    //l.setConfig(c);

    //for (int i = 0; i < 100000; ++i)
    //{
    //    LOG_TRACE(Logger::llNormal, "v: %d", i);
    //}
}
