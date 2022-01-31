#pragma once

#include "Math/Vector.h"

namespace Realisim
{
namespace Geometry
{
    //
    // direction is always a unit vector;
    class LineSegment
    {
    public:
        LineSegment() = default;
        LineSegment(const Math::Vector3& p0, const Math::Vector3& p1) : mP0(p0), mP1(p1) {}
        LineSegment(const LineSegment&) = default;
        LineSegment& operator=(const LineSegment&) = default;
        ~LineSegment() = default;

        //Math::Vector3 distance(const Math::Vector3& p0) const;
        bool isPointOnSegment(const Math::Vector3& p0) const
        {
            bool r = true;
            /*on verifie que le point d'intersection est sur le segment de ligne.
            x est le point d'intersection
            p1 le premier point du segment
            p2 le deuxieme point du segment

            On fait la projection du point x sur le segment de ligne et.
            (x1-p1)*(p2-p1) doit etre entre 0 et 1.
            */
            const Math::Vector3 a(p0 - mP0);
            const Math::Vector3 b(mP1 - mP0);
            double proj = (a*b) / (b*b);
            if (proj < 0.0 || 1.0  < proj)
            {
                r = false;
            }
            return r;
        }

        double length() const {
            return (mP1 - mP0).norm();
        }

        double lengthSquared() const {
            return (mP1 - mP0).normSquared();
        }

        //--- data
        Math::Vector3 mP0, mP1;
    };
}
}