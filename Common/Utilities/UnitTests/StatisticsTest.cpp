#include "gtest/gtest.h"
#include "Math/IsEqual.h"
#include "Utilities/Statistics.h"

using namespace Realisim;
using namespace Utilities;


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
    EXPECT_FLOAT_EQ(stats.getMean(), -8.20438);
    EXPECT_FLOAT_EQ(stats.getStandardDeviation(), 52.795);
}