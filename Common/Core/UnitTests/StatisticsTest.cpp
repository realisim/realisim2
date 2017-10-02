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
    EXPECT_DOUBLE_EQ(stats.minimum(), -69.01171);
    EXPECT_DOUBLE_EQ(stats.maximum(), 79.56491);
    EXPECT_FLOAT_EQ( (float)stats.average(), -8.20438f);
    EXPECT_FLOAT_EQ( (float)stats.standardDeviation(), 52.795f);
}

TEST(Statistics, csv)
{
	Statistics stats;
	stats.keepSamples(true);

	stats.add(-53.45981);
	stats.add(12.95491);
	stats.add(31.83041);
	stats.add(-53.91171);
	stats.add(79.56491);
	stats.add(-69.01171);
	stats.add(-37.77311);
	stats.add(24.1711);

	const std::string r = "Samples\n"
"-53.4598\n"
"12.9549\n"
"31.8304\n"
"-53.9117\n"
"79.5649\n"
"-69.0117\n"
"-37.7731\n"
"24.1711\n"
"\n"
"\n"
",Number of samples,average,minimum,maximum,standard deviation\n"
",8,-8.20438,-69.0117,79.5649,52.795";

	EXPECT_STREQ(r.c_str(), stats.toCsv().c_str());
	
}