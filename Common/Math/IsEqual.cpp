#include "IsEqual.h"

namespace Realisim
{
namespace Math
{
    bool isEqual(double a, double b, double iEpsilon)
    {
        double d = a - b;
        if(d<0.0) d = -d;
        return ( d <= iEpsilon );
    }

    bool isEqual(float a, float b, float iEpsilon)
    {
        float d = a - b;
        if(d<0.f) d = -d;
        return ( d <= iEpsilon );
    }
}
}