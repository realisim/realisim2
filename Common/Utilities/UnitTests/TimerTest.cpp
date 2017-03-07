#include "gtest/gtest.h"
#include "Utilities/Timer.h"

#include <thread>

using namespace Realisim;
using namespace Utilities;

TEST(Timer, test)
{
    {
        Timer t;
        std::chrono::milliseconds waitTime(250);
        std::this_thread::sleep_for(waitTime);
        double e = t.getElapsed();
        EXPECT_GE(e, 0.250);
    }
    
    {
        Timer t;
        std::chrono::milliseconds waitTime(10);
        std::this_thread::sleep_for(waitTime);
        double e = t.getElapsed();
        EXPECT_GE(e, 0.010);
    }
    
    {
        Timer t;
        std::chrono::seconds waitTime(1);
        std::this_thread::sleep_for(waitTime);
        double e = t.getElapsed();
        EXPECT_GE(e, 1);
    }
}