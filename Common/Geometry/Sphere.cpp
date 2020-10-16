#include <cassert>
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
void Sphere::makeFaces(Mesh *ipMesh, int iN, int iVertexOffset, bool iReverseOrder) const
{
    for(int j = 0; j < iN - 1; ++j)
        for (int i = 0; i < iN - 1; ++i)
        {
            const int llIndex = iVertexOffset + (j*iN + i);
            const int lrIndex = llIndex + 1;

            const int ulIndex = iVertexOffset + ((j+1)*iN + i);
            const int urIndex = ulIndex + 1;

            if (!iReverseOrder)
            {
                ipMesh->makeFace( {(uint16_t)llIndex, (uint16_t)lrIndex, (uint16_t)urIndex} );
                ipMesh->makeFace( {(uint16_t)llIndex, (uint16_t)urIndex, (uint16_t)ulIndex} );
            }
            else
            {
                ipMesh->makeFace( {(uint16_t)llIndex, (uint16_t)ulIndex, (uint16_t)urIndex} );
                ipMesh->makeFace( {(uint16_t)llIndex, (uint16_t)urIndex, (uint16_t)lrIndex} );
            }
            
        }
}

//-----------------------------------------------------------------------------
// Here, we make a 6 face box and normalize it, so it becomes a sphere
//
//
Mesh Sphere::makeMesh() const
{
    Mesh mesh;
    mesh.setNumberOfVerticesPerFace(3);

    //-- grab references to guts of mesh
    vector<Mesh::VertexData>& vertices = mesh.getVerticesRef();

    const int gridSize = 5; // must be odd
    const int n = gridSize / 2;
    const int numVerticesPerPlane = gridSize*gridSize;
    int vertexOffsetForFaces = 0;

    //add all vertices
    Mesh::VertexData vd;
    //top y
    for(int j = -n; j <= n; ++j)
        for (int i = -n; i <= n; ++i)
        { 
            Vector3 v(i, n, j);
            v.normalize();
            v *= mRadius;
            v += mCenter;
            vd.mVertex = v;
            vertices.push_back(vd); 
        }

    // make faces
    makeFaces(&mesh, gridSize, vertexOffsetForFaces, true);
    vertexOffsetForFaces += numVerticesPerPlane;

    //bottom -y
    for(int j = -n; j <= n; ++j)
        for (int i = -n; i <= n; ++i)
        {
            Vector3 v(i, -n, j);
            v.normalize();
            v *= mRadius;
            v += mCenter;
            vd.mVertex = v;
            vertices.push_back(vd); 
        }
    // make faces
    makeFaces(&mesh, gridSize, vertexOffsetForFaces, false);
    vertexOffsetForFaces += numVerticesPerPlane;

    //left -x
    for(int j = -n; j <= n; ++j)
        for (int i = -n; i <= n; ++i)
        {
            Vector3 v(-n, i, j);
            v.normalize();
            v *= getRadius();
            v += mCenter;
            vd.mVertex = v;
            vertices.push_back(vd); 
        }
    // make faces
    makeFaces(&mesh, gridSize, vertexOffsetForFaces, true);
    vertexOffsetForFaces += numVerticesPerPlane;

    //right x
    for(int j = -n; j <= n; ++j)
        for (int i = -n; i <= n; ++i)
        {
            Vector3 v(n, i, j);
            v.normalize();
            v *= getRadius();
            v += mCenter;
            vd.mVertex = v;
            vertices.push_back(vd); 
        }
    // make faces
    makeFaces(&mesh, gridSize, vertexOffsetForFaces, false);
    vertexOffsetForFaces += numVerticesPerPlane;

    //back -z
    for(int j = -n; j <= n; ++j)
        for (int i = -n; i <= n; ++i)
        {
            Vector3 v(i, j, -n);
            v.normalize();
            v *= getRadius();
            v += mCenter;
            vd.mVertex = v;
            vertices.push_back(vd); 
        }
    // make faces
    makeFaces(&mesh, gridSize, vertexOffsetForFaces, true);
    vertexOffsetForFaces += numVerticesPerPlane;

    //front z
    for(int j = -n; j <= n; ++j)
        for (int i = -n; i <= n; ++i)
        {
            Vector3 v(i, j, n);
            v.normalize();
            v *= getRadius();
            v += mCenter;
            vd.mVertex = v;
            vertices.push_back(vd); 
        }
    // make faces
    makeFaces(&mesh, gridSize, vertexOffsetForFaces, false);
    vertexOffsetForFaces += numVerticesPerPlane;

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
