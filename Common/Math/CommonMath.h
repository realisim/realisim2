#pragma once

#include <cmath>

#include "IsEqual.h"

// remove warning if function not used
#define SUPPRESS_WARNING(a) (void)a


static inline const double radiansToDegrees(const double& d )
{
    SUPPRESS_WARNING(radiansToDegrees);
	return d*(180./M_PI);
}

static inline const double degreesToRadians(const double& d)
{
    SUPPRESS_WARNING(degreesToRadians);
	return d*(M_PI/180.);
}

static inline const double degreesMinutesSecondsToDegrees(const int& degree,const int& minutes,const double& seconds)
{
    SUPPRESS_WARNING(degreesMinutesSecondsToDegrees);
	// take out sign
	const double absoluteDegree = abs(degree);
	return std::copysign((double)absoluteDegree + ((double)minutes/60.0) + ((double)seconds/3600.0),degree);
}
static inline void degreesToDegreesMinutesSeconds(const double& decimalDegrees,int& degree,int& minutes,double& seconds)
{
    SUPPRESS_WARNING(degreesToDegreesMinutesSeconds);
	// take out sign
	const double absoluteDegree = std::abs(decimalDegrees);

	degree = (int)absoluteDegree;
	minutes = (int)((absoluteDegree-degree)*60.0);
	seconds = ((absoluteDegree-degree - (minutes/60.0)) * 3600.0);

	// put back the sign in degree
	degree = (int)std::copysign((double)degree,decimalDegrees);
}
