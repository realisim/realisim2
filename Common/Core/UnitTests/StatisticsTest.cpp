#include "gtest/gtest.h"
#include "Math/IsEqual.h"
#include "Core/Statistics.h"

using namespace Realisim;
using namespace Core;


TEST(Statistics, general)
{
    Statistics stats;
    
    stats.add(-53.45981);
    stats.add(12.95491);
    stats.add(31.83041);
    stats.add(-53.91171);
    stats.add(79.56491);
    stats.add(-69.01171);
    stats.add(-37.77311);
    stats.add(24.1711);
    
    //verified on wolfram
    EXPECT_DOUBLE_EQ(stats.getMin(), -69.01171);
    EXPECT_DOUBLE_EQ(stats.getMax(), 79.56491);
    EXPECT_FLOAT_EQ((float)stats.getMean(), -8.20438f);
    EXPECT_FLOAT_EQ((float)stats.getStandardDeviation(), 52.795f);
}