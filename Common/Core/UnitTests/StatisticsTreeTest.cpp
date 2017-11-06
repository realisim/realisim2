#include "gtest/gtest.h"
#include "Math/IsEqual.h"
#include "Core/StatisticsTree.h"
#include <set>
#include <string>

using namespace Realisim;
    using namespace Core;
using namespace std;


TEST(StatisticsTree, general)
{
    StatisticsTree statTree;
    
    statTree.add("a", 1);
    statTree.add("a/b", 2);
    statTree.add("a/b/c", 3);
    statTree.add("a/b/c", 4);
    statTree.add("a/c", 4);
    statTree.add("b", 1);
    statTree.add("b", 2);
    statTree.add("b/c", 2);
    statTree.add("b/c", 3);
    statTree.add("b/c/d", 3);
    
    EXPECT_EQ(statTree.getNumberOfKeys(), 7);
    
    EXPECT_TRUE(statTree.hasKey("a"));
    EXPECT_TRUE(statTree.hasKey("a/b"));
    EXPECT_TRUE(statTree.hasKey("a/b/c"));
    EXPECT_TRUE(statTree.hasKey("a/c"));
    EXPECT_TRUE(statTree.hasKey("b"));
    EXPECT_TRUE(statTree.hasKey("b/c"));
    EXPECT_TRUE(statTree.hasKey("b/c/d"));
    
    EXPECT_TRUE(statTree.hasChilds("a"));
    EXPECT_TRUE(statTree.hasChilds("b"));
    EXPECT_FALSE(statTree.hasChilds("a/b/c"));
    EXPECT_FALSE(statTree.hasChilds("b/c/d"));
    
    EXPECT_DOUBLE_EQ(statTree.getAverage("a"), 1);
    EXPECT_DOUBLE_EQ(statTree.getMaximum("a"), 1);
    EXPECT_DOUBLE_EQ(statTree.getMinimum("a"), 1);
   // EXPECT_DOUBLE_EQ(statTree.getStandardDeviation("a"), nan);
    
    EXPECT_DOUBLE_EQ(statTree.getAverage("a/b/c"), 3.5);
    EXPECT_DOUBLE_EQ(statTree.getMaximum("a/b/c"), 4);
    EXPECT_DOUBLE_EQ(statTree.getMinimum("a/b/c"), 3);
    EXPECT_DOUBLE_EQ(statTree.getStandardDeviation("a/b/c"), 0.70710678118654757);
    

    int i = 0;
    vector<double> results = {1, 2, 3.5, 4, 1.5, 2.5, 3};
    const set<string>& keys = statTree.getKeys();
    for(auto key : keys)
    {
        EXPECT_DOUBLE_EQ(statTree.getAverage(key), results[i]);
        ++i;
    }

    statTree.clear();
    EXPECT_EQ(statTree.getNumberOfKeys(), 0);
}
