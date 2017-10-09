#include <cassert>
#include <cmath>
#include "Sphere.h"
#include <vector>

using namespace Realisim;
    using namespace Geometry;
    using namespace Math;
using namespace std;

//-----------------------------------------------------------------------------
Sphere::Sphere():
    mCenter(),
    mRadius(0.0)
{}

//-----------------------------------------------------------------------------
Sphere::Sphere(const Math::Vector3& iCenter, double iRadius) :
    mCenter(iCenter),
    mRadius(iRadius)
{}

//-----------------------------------------------------------------------------
bool Sphere::contains(const Math::Vector3& iPoint, bool iProper /*= false*/) const
{
    //(x - x0)^2 + ( y - y0 )^2 + (z - z0)^2 = r^2
    //   a^2     +     b^2      +     c^2    = r^2

    const Vector3 m = getCenter();
    const double r = getRadius();
    const double a = (iPoint.x() - m.x());
    const double b = (iPoint.y() - m.y());
    const double c = (iPoint.z() - m.z());

    if(iProper)
        return (a*a + b*b + c*c) < r*r;
    else
        return (a*a + b*b + c*c) <= r*r;

}

//-----------------------------------------------------------------------------
// distance to the sphere surface, positive = outside, negative = inside
double Sphere::distanceToBound(const Math::Vector3& p) const
{
    return (p - getCenter()).norm() - getRadius();
}


//-----------------------------------------------------------------------------
double Sphere::getArea() const
{ return 4.0 * M_PI * getRadius() * getRadius(); }

//-----------------------------------------------------------------------------
Math::Vector3 Sphere::getCenter() const
{ return mCenter; }

//-----------------------------------------------------------------------------
double Sphere::getRadius() const
{ return mRadius; }

//-----------------------------------------------------------------------------
double Sphere::getVolume() const
{ 
    const double r = getRadius();
    return 4.0 * M_PI * r * r * r / 3.0; 
}

//-----------------------------------------------------------------------------
bool Sphere::isValid() const
{ return mRadius >= 0.0; }

//-----------------------------------------------------------------------------
Mesh Sphere::makeMesh() const
{
    Mesh mesh;
    mesh.setNumberOfVerticesPerFace(3);

    //-- grab references to guts of mesh
    vector<Vector3>& vertices = mesh.getVertices();
//    vector<Mesh::Face>& faces = mesh.getFaces();
//    vector<int>& triangulatedFaceIndices = mesh.getFaceIndices();

    const int n = 5;

    //add all vertices

    //top y
    for(int j = 0; j < n; ++j)
        for (int i = 0; i < n; ++i)
        { 
            Vector3 v(i, 1, j);
            v.normalize();
            v *= getRadius();
            vertices.push_back(v); 
        }

    //bottom -y
    for (int j = 0; j < n; ++j)
        for (int i = 0; i < n; ++i)
        {
            Vector3 v(i, -1, j);
            v.normalize();
            v *= getRadius();
            vertices.push_back(v);
        }

    //left -x
    for (int j = 0; j < n; ++j)
        for (int i = 0; i < n; ++i)
        {
            Vector3 v(-1, i, j);
            v.normalize();
            v *= getRadius();
            vertices.push_back(v);
        }

    //right x
    for (int j = 0; j < n; ++j)
        for (int i = 0; i < n; ++i)
        {
            Vector3 v(1, i, j);
            v.normalize();
            v *= getRadius();
            vertices.push_back(v);
        }

    //back -z
    for (int j = 0; j < n; ++j)
        for (int i = 0; i < n; ++i)
        {
            Vector3 v(i, j, -1);
            v.normalize();
            v *= getRadius();
            vertices.push_back(v);
        }

    //front z
    for (int j = 0; j < n; ++j)
        for (int i = 0; i < n; ++i)
        {
            Vector3 v(i, j, 1);
            v.normalize();
            v *= getRadius();
            vertices.push_back(v);
        }

    return mesh;
}

//-----------------------------------------------------------------------------
double Sphere::projectedDistanceToBound(const Math::Vector3& p, const Math::Vector3& direction) const
{
    return (getCenter() - p)*direction - getRadius();
}

//-----------------------------------------------------------------------------
void Sphere::setCenter(const Math::Vector3& iCenter)
{ mCenter = iCenter; }

//-----------------------------------------------------------------------------
void Sphere::setRadius(double iRadius)
{mRadius = iRadius;}
