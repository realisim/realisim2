#include "Triangle.h"

using namespace Realisim;
    using namespace Geometry;
    using namespace Math;

// STATIC
Math::Vector3 Triangle::mDummyVertex;

//-----------------------------------------------------------------------------
Triangle::Triangle() : 
    mX(3,Vector3(0.0)),
    mNormal( Vector3(0.0) )
{}

//-----------------------------------------------------------------------------
Triangle::Triangle(const Math::Vector3& iP0, const Math::Vector3& iP1, const Math::Vector3& iP2) :
    mX(3, Vector3(0.0))
{
    set(iP0, iP1, iP2);
}

//-----------------------------------------------------------------------------
Triangle::~Triangle() {}

//-----------------------------------------------------------------------------
bool Triangle::contains(const Vector3 &iP) const
{
    const Vector3 a = (mX[1] - mX[0]) ^ (iP - mX[0]);
    const Vector3 b = (mX[2] - mX[1]) ^ (iP - mX[1]);
    const Vector3 c = (mX[0] - mX[2]) ^ (iP - mX[2]);

    return (a*mNormal >= 0 && b*mNormal >= 0 && c*mNormal >= 0);
}

//-----------------------------------------------------------------------------
double Triangle::getArea() const
{
    double r = 0.0;

    if (isValid())
    {
        const double a = (mX[1] - mX[0]).norm();
        const double b = (mX[2] - mX[1]).norm();
        const double c = (mX[0] - mX[2]).norm();

        const double s = (a + b + c) * 0.5;
        r = sqrt(s*(s - a)*(s - b)*(s - c));
    }
    return r;
}

//-----------------------------------------------------------------------------
// see https://en.wikipedia.org/wiki/Barycentric_coordinate_system
//    vector form: https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/barycentric-coordinates
//    https://www.geogebra.org/m/ZuvmPjmy
std::array<double, 3> Triangle::getBarycentricCoefficients(const Vector3& iP) const
{
    std::array<double, 3> coeff;
    //        C
    //     /    \
    //    /      \
    //   /    P   \
    //  /          \
    // A-------------B

    // P = wA + uB + vC
    // u = triangleCAParea / triangleABCarea
    // v = triangleABParea / triangleABCarea
    // w = 1 - u - v

    // triangle area => parallelogramArea * 0.5 => parallelogramArea = || (B-A) cross (C-A) ||
    //
    //const double denomInv = 1.0 / ((mX[1] - mX[0]) ^ (mX[2] - mX[0])).norm();

    // take sign into account...
    const Vector3 caCrossPc = (mX[0] - mX[2]) ^ (iP - mX[2]);
    const Vector3 baCrossPa = (mX[1] - mX[0]) ^ (iP - mX[0]);

    double sign = caCrossPc * mNormal >= 0 ? 1.0 : -1.0;
    const double u = sign * caCrossPc.norm() * mBarycentricDenominatorInv;

    sign = baCrossPa * mNormal >= 0 ? 1.0 : -1.0;
    const double v = sign * baCrossPa.norm() * mBarycentricDenominatorInv;
    const double w = 1 - u - v;
    coeff[0] = w;
    coeff[1] = u;
    coeff[2] = v;
    return coeff;
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
            (mX[0].x() + mX[1].x() + mX[2].x()) / 3.0,
            (mX[0].y() + mX[1].y() + mX[2].y()) / 3.0,
            (mX[0].z() + mX[1].z() + mX[2].z()) / 3.0);
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
    return mX;
}

//-----------------------------------------------------------------------------
std::vector<Math::Vector3>& Triangle::getVerticesRef()
{
    return mX;
}

//-----------------------------------------------------------------------------
const Math::Vector3& Triangle::getVertex(int iIndex) const
{
    const Vector3 *r = &mDummyVertex;
    if (iIndex >= 0 && iIndex < 3)
    {
        r = &(mX[iIndex]);
    }
    return *r;
}

//-----------------------------------------------------------------------------
bool Triangle::isValid() const
{
    return mX[0] != mX[1] &&
        mX[1] != mX[2] &&
        mX[2] != mX[0];
}

//-----------------------------------------------------------------------------
void Triangle::set(const Math::Vector3& iP0, const Math::Vector3& iP1, const Math::Vector3& iP2)
{
    mX[0] = iP0;
    mX[1] = iP1;
    mX[2] = iP2;

    const Vector3 v0 = mX[1] - mX[0];
    const Vector3 v1 = mX[2] - mX[0];
    mNormal = v0 ^ v1;
    
    mBarycentricDenominatorInv = 1.0 / mNormal.norm();
    mNormal.normalize();
}

//-----------------------------------------------------------------------------
Math::Vector3 Triangle::toBarycentric(const Math::Vector3& iP) const
{
    const std::array<double, 3> c = getBarycentricCoefficients(iP);
    return (c[0]*mX[0] + c[1]*mX[1] + c[2]*mX[2]);
}
