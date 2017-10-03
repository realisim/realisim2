#include "gtest/gtest.h"
#include "Math/IsEqual.h"
#include "Math/CommonMath.h"
#include <cmath>

TEST(CommonAngleConversion,RadiansToDegree)
{
	EXPECT_DOUBLE_EQ(radiansToDegrees(0.65),37.24225668350351);
}

TEST(CommonAngleConversion,DegreeToRadian)
{
	EXPECT_DOUBLE_EQ(degreesToRadians(96.4),1.682497398922534);
}

TEST(CommonAngleConversion,DegreeToRadianToDegree)
{
	double a = -64.7;
	double b = degreesToRadians(a);
	EXPECT_DOUBLE_EQ(radiansToDegrees(b),a);
}

TEST(CommonAngleConversion,RadiansToDegreeToRadian)
{
	double a = -0.02647;
	double b = radiansToDegrees(a);
	EXPECT_DOUBLE_EQ(degreesToRadians(b),a);
}

TEST(CommonAngleConversion,FromDegMinSecond)
{
	int degrees = 38;
	int minutes = 42;
	double secondes = 25.1;
	double decimalDegrees = degreesMinutesSecondsToDegrees(degrees,minutes,secondes);
	EXPECT_DOUBLE_EQ(decimalDegrees, 38.70697222222222);

	degrees = -55;
	minutes = 12;
	secondes = 5;
	decimalDegrees = degreesMinutesSecondsToDegrees(degrees,minutes,secondes);

	// we don't have enough precision with external validation tools to use EXPECT_FLOAT_EQ
	EXPECT_TRUE(Realisim::Math::isEqual(decimalDegrees, -55.20138889,0.00000001));
}

TEST(CommonAngleConversion,ToDegMinSecond)
{
	int degrees = 0;
	int minutes = 0;
	double secondes = 0;
	double decimalDegrees = 13.123459;
	degreesToDegreesMinutesSeconds(decimalDegrees,degrees,minutes,secondes);
	EXPECT_EQ(degrees,13);
	EXPECT_EQ(minutes,7);
	EXPECT_FLOAT_EQ((float)secondes,24.4524000f);

	degrees = 0;
	minutes = 0;
	secondes = 0;
	decimalDegrees = -67.539841;
	degreesToDegreesMinutesSeconds(decimalDegrees,degrees,minutes,secondes);
	EXPECT_EQ(degrees,-67);
	EXPECT_EQ(minutes,32);
	EXPECT_FLOAT_EQ((float)secondes,23.4276f);
}