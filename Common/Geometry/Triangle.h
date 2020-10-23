
#pragma once

#include <array>
#include "Math/Vector.h"
#include <vector>

namespace Realisim
{
namespace Geometry
{
    class Triangle
    {
    public:
        Triangle();
        Triangle(const Math::Vector3& iP0, const Math::Vector3& iP1, const Math::Vector3& iP2);

        ~Triangle();
        Triangle(const Triangle&) = default;
        Triangle& operator=(const Triangle&) = default;

        bool contains(const Math::Vector3&) const;
        double getArea() const;
        std::array<double, 3> getBarycentricCoefficients(const Math::Vector3& iP) const;
        Math::Vector3 getCentroid() const;
        const Math::Vector3& getNormal() const;
        const std::vector<Math::Vector3>& getVertices() const;
        std::vector<Math::Vector3>& getVerticesRef();
        const Math::Vector3& getVertex(int iIndex) const;
        bool isValid() const;
        void set(const Math::Vector3& iP0, const Math::Vector3& iP1, const Math::Vector3& iP2);
        Math::Vector3 toBarycentric(const Math::Vector3& iP) const;

    protected:
        std::vector<Math::Vector3> mX;
        Math::Vector3 mNormal;
        static Math::Vector3 mDummyVertex;
        double mBarycentricDenominatorInv; //optimization
    };
}
}