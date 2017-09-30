#pragma once

#include <cmath>

#include "IsEqual.h"

// remove warning if function not used
#pragma warning(push)
#pragma warning(disable : 4505)


static const double radiansToDegrees(const double& d )
{
	return d*(180./M_PI);
}

static const double degreesToRadians(const double& d)
{
	return d*(M_PI/180.);
}

static const double degreesMinutesSecondsToDegrees(const int& degree,const int& minutes,const double& seconds)
{
	// take out sign
	const double absoluteDegree = abs(degree);
	return std::copysign((double)absoluteDegree + ((double)minutes/60.0) + ((double)seconds/3600.0),degree);
}
static void degreesToDegreesMinutesSeconds(const double& decimalDegrees,int& degree,int& minutes,double& seconds)
{
	// take out sign
	const double absoluteDegree = abs(decimalDegrees);

	degree = (int)absoluteDegree;
	minutes = (int)((absoluteDegree-degree)*60.0);
	seconds = ((absoluteDegree-degree - (minutes/60.0)) * 3600.0);

	// put back the sign in degree
	degree = (int)std::copysign((double)degree,decimalDegrees);
}

#pragma warning(pop)