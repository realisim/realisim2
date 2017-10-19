#pragma once

#include "Math/Vector.h"

namespace Realisim
{
namespace LightBeam
{
    class Light
    {
    public:
        Light();
        Light(const Light&) = default;
        Light& operator=(const Light&) = default;
        virtual ~Light() = default;

        enum Type {tPoint, tSpot, tDirectionnal, tArea};

        Type getType() const;
        void setType(Type iType);

    protected:
        Type mType;
        Math::Vector3 mPosition;
    };
}
}