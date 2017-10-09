#pragma once

namespace Realisim
{
namespace Math
{
    
    inline double lerp(double iBegin, double iEnd, double iFactor)
    { return iBegin + (iEnd - iBegin)*iFactor; }
    
    //double bilerp(double iBegin, double iEnd, double iFactor);
    //double trilerp(double iBegin, double iEnd, double iFactor);
}
}