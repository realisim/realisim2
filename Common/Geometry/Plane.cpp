
#include "Geometry/Utilities.h"
#include "Math/IsEqual.h"
#include "Math/VectorI.h"
#include "Plane.h"
#include <vector>

using namespace Realisim;
    using namespace Geometry;
    using namespace Math;
using namespace std;

//------------------------------------------------------------------------------
Plane::Plane() :
    mIsValid(true),
    mPoint(0.0),
    mNormal(0.0, 1.0, 0.0)
{}

//------------------------------------------------------------------------------
Plane::Plane(const Vector3& iP, const Vector3& iN) :
    mIsValid(false),
    mPoint(iP),
    mNormal(iN)
{
    validate();
}

//------------------------------------------------------------------------------
Plane::Plane(const Math::Vector3& p0,
    const Math::Vector3& p1, 
    const Math::Vector3& p2) :
    mIsValid(false),
    mPoint(0.0),
    mNormal(0.0, 0.0, 0.0)
{
    set(p0, p1, p2);
}

//------------------------------------------------------------------------------
Plane::~Plane()
{}

//------------------------------------------------------------------------------
// return the closest distance to the plane.
// A positif distance means the point is on the side front side of the plane.
// (The normal is on the front side).
// 
// A negative distance means the point is on the back side of the plane
double Plane::distance(const Math::Vector3& p) const
{
    Math::Vector3 v = p-getPoint();

    double d = v*getNormal(); // dot

    return d;
}

//------------------------------------------------------------------------------
const Vector3& Plane::getPoint() const
{
    return mPoint;
}

//------------------------------------------------------------------------------
// returns the normalized normal to the plane.
//
const Vector3& Plane::getNormal() const
{
    return mNormal;
}

//------------------------------------------------------------------------------
bool Plane::isValid() const
{
    return mIsValid;
}

//------------------------------------------------------------------------------
Mesh Plane::makeMesh(const Vector2& iSizeInMeter) const
{
    Mesh mesh;

    mesh.setNumberOfVerticesPerFace(3);

    //-- grab references to guts of mesh
    Vector3 someX = getPerpendicularVector(getNormal());
    Vector3 someY = getNormal() ^ someX;
    someX.normalize();
    someY.normalize();

    //4 vertices
    const Vector3& p = getPoint();
    const Vector2 halfSize(iSizeInMeter.x() / 2.0, iSizeInMeter.y() / 2.0);

    Mesh::VertexData vd0, vd1, vd2, vd3;
    vd0.mVertex = p - (someX * halfSize.x()) - (someY * halfSize.y());
    vd1.mVertex = p + (someX * halfSize.x()) - (someY * halfSize.y());
    vd2.mVertex = p + (someX * halfSize.x()) + (someY * halfSize.y());
    vd3.mVertex = p - (someX * halfSize.x()) + (someY * halfSize.y());

    vd0.mLayerIndexToTextureCoordinates[0] = Vector2(0.0, 0.0);
    vd1.mLayerIndexToTextureCoordinates[0] = Vector2(1.0, 0.0);
    vd2.mLayerIndexToTextureCoordinates[0] = Vector2(1.0, 1.0);
    vd3.mLayerIndexToTextureCoordinates[0] = Vector2(0.0, 1.0);

    vector<Mesh::VertexData>& vertices = mesh.getVerticesRef();
    vertices = { vd0, vd1, vd2, vd3 };
    
    mesh.makeFace({ 0, 1, 2 });
    mesh.makeFace({ 2, 3, 0 });

    mesh.generateFlatNormals();

    return mesh;
}

//------------------------------------------------------------------------------
void Plane::set(const Math::Vector3& p, const Math::Vector3& n)
{
    mPoint = p;
    mNormal = n;
    validate();
}

//------------------------------------------------------------------------------
void Plane::set(const Math::Vector3& p0,
    const Math::Vector3& p1,
    const Math::Vector3& p2)
{
    Vector3 v0 = p1 - p0;
    Vector3 v1 = p2 - p0;
    set(p0, v0 ^ v1);
}

//-----------------------------------------------------------------------------
void Plane::validate()
{
    mIsValid = !mNormal.isEqual(Vector3(0.0, 0.0, 0.0), 1e-13);
    if (mIsValid)
    {
        mNormal.normalize();
    }
}