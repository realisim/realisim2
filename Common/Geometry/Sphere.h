#pragma once
#include "Math/Vector.h"
#include "Mesh.h"

namespace Realisim
{
namespace Geometry
{
    /*  This class represent a mathematical sphere.
        It is defined by a point (center) and a radius.
    */
    class Sphere
    {
    public:
        Sphere();
        Sphere(const Math::Vector3& iCenter, double iRadius);
        Sphere(const Sphere&) = default;
        Sphere& operator= (const Sphere&) = default;
        virtual ~Sphere() = default;

        bool contains(const Math::Vector3& iPoint, bool iProper = false) const;
        double distanceToBound(const Math::Vector3& p) const;
        double getArea() const;
        Math::Vector3 getCenter() const;
        double getRadius() const;
        double getVolume() const;
        virtual bool isValid() const;
        Mesh makeMesh() const;
        double projectedDistanceToBound(const Math::Vector3& p, const Math::Vector3& direction) const;
        void setCenter(const Math::Vector3& iCenter);
        void setRadius(double);

    protected:
        void makeFaces(Mesh *ipMesh, int iN, int iVertexOffset, bool iReverseOrder) const;

        Math::Vector3 mCenter;
        double mRadius;
    };

}
}