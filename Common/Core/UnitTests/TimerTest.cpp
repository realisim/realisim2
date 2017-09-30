#include "gtest/gtest.h"
#include "Core/Timer.h"

#include <thread>

using namespace Realisim;
using namespace Core;

TEST(Timer, test)
{
    {
        Timer t;
        std::chrono::milliseconds waitTime(250);
        std::this_thread::sleep_for(waitTime);
        double e = t.elapsed();
        EXPECT_GE(e, 0.250);
    }
    
    {
        Timer t;
        std::chrono::milliseconds waitTime(10);
        std::this_thread::sleep_for(waitTime);
        double e = t.elapsed();
        EXPECT_GE(e, 0.010);
    }
    
    {
        Timer t;
        std::chrono::seconds waitTime(1);
        std::this_thread::sleep_for(waitTime);
        double e = t.elapsed();
        EXPECT_GE(e, 1);
    }
}