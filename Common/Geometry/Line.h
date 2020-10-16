#pragma once

#include "Math/Vector.h"

namespace Realisim
{
namespace Geometry
{
    //
    // direction is always a unit vector;
    class Line
    {
    public:
        Line();
        Line(const Math::Vector3& p0, const Math::Vector3& p1);
        Line(const Line&) = default;
        Line& operator=(const Line&) = default;
        ~Line() = default;

        //Math::Vector3 distance(const Math::Vector3& p0) const;
        const Math::Vector3& getDirection() const;
        const Math::Vector3& getOrigin() const;
        bool isValid() const;
        void set(const Math::Vector3& p0, const Math::Vector3& p1);
        void setDirection(const Math::Vector3& iD);
        void setOrigin(const Math::Vector3& p0);

    protected:
        Math::Vector3 mOrigin;
        Math::Vector3 mDirection;
    };
}
}