#pragma once

#include <cmath>

#include "Math/IsEqual.h"
#include "Math/Vector.h"

// remove warning if function not used
#pragma warning(push)
#pragma warning(disable : 4505)

namespace Realisim
{
    //---------------------------------------------------------------------------------------------------------------------
    template<typename T>
    static T clamp(T iV, T iMin, T iMax)
    {
        return std::min(std::max(iV, iMin), iMax);
    }

    //---------------------------------------------------------------------------------------------------------------------
    static inline const double radiansToDegrees(const double& d)
    {
        return d * (180. / M_PI);
    }

    //---------------------------------------------------------------------------------------------------------------------
    static inline const double degreesToRadians(const double& d)
    {
        return d * (M_PI / 180.);
    }

    //---------------------------------------------------------------------------------------------------------------------
    static inline const double degreesMinutesSecondsToDegrees(const int& degree, const int& minutes, const double& seconds)
    {
        // take out sign
        const double absoluteDegree = abs(degree);
        return std::copysign((double)absoluteDegree + ((double)minutes / 60.0) + ((double)seconds / 3600.0), degree);
    }

    //---------------------------------------------------------------------------------------------------------------------
    static inline void degreesToDegreesMinutesSeconds(const double& decimalDegrees, int& degree, int& minutes, double& seconds)
    {
        // take out sign
        const double absoluteDegree = std::abs(decimalDegrees);

        degree = (int)absoluteDegree;
        minutes = (int)((absoluteDegree - degree) * 60.0);
        seconds = ((absoluteDegree - degree - (minutes / 60.0)) * 3600.0);

        // put back the sign in degree
        degree = (int)std::copysign((double)degree, decimalDegrees);
    }

    //---------------------------------------------------------------------------------------------------------------------
    // http://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
    static unsigned int nextPowerOf2(unsigned int v)
    {
        v--;
        v |= v >> 1;
        v |= v >> 2;
        v |= v >> 4;
        v |= v >> 8;
        v |= v >> 16;
        v++;
        return v;
    }

    //---------------------------------------------------------------------------------------------------------------------
    static Math::Vector3 getPerpendicularVector(const Math::Vector3& iV)
    {
        Math::Vector3 r;

        r = iV.x() == iV.y() ?
            Math::Vector3(-iV.z(), 0.0, iV.x()) :
            Math::Vector3(-iV.y(), iV.x(), 0.0);

        return r;
    }
}


#pragma warning(pop)
