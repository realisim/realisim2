#pragma once

#include "Core/Color.h"
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
        virtual ~Light();

        enum Type {tPoint, tSpot, tDirectionnal, tArea};
        enum AttenuationType {atNone, atLinear, atQuadratic};

        AttenuationType getAttenuationType() const;
        Core::Color getColor() const;
        Math::Vector3 getDirection() const;
        Math::Vector3 getPosition() const;
        Type getType() const;
        void setAttenuationType(AttenuationType iType);
        void setColor(const Core::Color&);
        void setDirection(const Math::Vector3&);
        void setPosition(const Math::Vector3&);
        void setType(Type iType);

    protected:
        Type mType;
        AttenuationType mAttenuationType;
        Math::Vector3 mPosition;
        Math::Vector3 mDirection;
        Core::Color mColor;
        //double mPower;
    };
}
}