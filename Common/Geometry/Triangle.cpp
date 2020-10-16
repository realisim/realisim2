#include "Triangle.h"

using namespace Realisim;
    using namespace Geometry;
    using namespace Math;

// STATIC
Math::Vector3 Triangle::mDummyVertex;

//-----------------------------------------------------------------------------
Triangle::Triangle() : 
    x(3,Vector3(0.0)),
    mNormal( Vector3(0.0) )
{}

//-----------------------------------------------------------------------------
Triangle::Triangle(const Math::Vector3& iP0, const Math::Vector3& iP1, const Math::Vector3& iP2) :
    x(3, Vector3(0.0))
{
    set(iP0, iP1, iP2);
}

//-----------------------------------------------------------------------------
Triangle::~Triangle() {}

//-----------------------------------------------------------------------------
double Triangle::getArea() const
{
    double r = 0.0;

    if (isValid())
    {
        const double a = (x[1] - x[0]).norm();
        const double b = (x[2] - x[1]).norm();
        const double c = (x[0] - x[2]).norm();

        const double s = (a + b + c) * 0.5;
        r = sqrt(s*(s - a)*(s - b)*(s - c));
    }
    return r;
}

//-----------------------------------------------------------------------------
//  https://www.mathsisfun.com/geometry/triangle-centers.html
//
Vector3 Triangle::getCentroid() const
{
    Vector3 r;

    if (isValid())
    {
        r.set( 
            (x[0].x() + x[1].x() + x[2].x()) / 3.0,
            (x[0].y() + x[1].y() + x[2].y()) / 3.0,
            (x[0].z() + x[1].z() + x[2].z()) / 3.0);
    }
    return r;
}

//-----------------------------------------------------------------------------
const Math::Vector3& Triangle::getNormal() const
{
    return mNormal;
}

//-----------------------------------------------------------------------------
const std::vector<Math::Vector3>& Triangle::getVertices() const
{
    return x;
}

//-----------------------------------------------------------------------------
std::vector<Math::Vector3>& Triangle::getVerticesRef()
{
    return x;
}

//-----------------------------------------------------------------------------
const Math::Vector3& Triangle::getVertex(int iIndex) const
{
    const Vector3 *r = &mDummyVertex;
    if (iIndex >= 0 && iIndex < 3)
    {
        r = &(x[iIndex]);
    }
    return *r;
}

//-----------------------------------------------------------------------------
bool Triangle::isValid() const
{
    return x[0] != x[1] &&
        x[1] != x[2] &&
        x[2] != x[0];
}

//-----------------------------------------------------------------------------
void Triangle::set(const Math::Vector3& iP0, const Math::Vector3& iP1, const Math::Vector3& iP2)
{
    x[0] = iP0;
    x[1] = iP1;
    x[2] = iP2;

    const Vector3 v0 = x[1] - x[0];
    const Vector3 v1 = x[2] - x[0];
    mNormal = v0 ^ v1;
    mNormal.normalize();
}

