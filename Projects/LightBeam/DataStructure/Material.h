#pragma once

#include "Core/Color.h"

namespace Realisim
{
namespace LightBeam
{
    class Material
    {
    public:
        Material() = default;
        Material(const Material&) = default;
        Material& operator=(const Material&) = default;
        ~Material() = default;

        Core::Color mColor;

    protected:

    };
}
}