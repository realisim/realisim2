#pragma once

#include <limits>

namespace Realisim
{
namespace Math
{
    bool isEqual(double a, double b, double iEpsilon = std::numeric_limits<double>::epsilon());
    bool isEqual(float a, float b, float iEpsilon = std::numeric_limits<float>::epsilon());
}
}