#include "AxisAlignedBoundingBox.h"
#include <cassert>
#include <vector>
#include <algorithm>

using namespace Realisim;
    using namespace Geometry;
    using namespace Math;
using namespace std;

//-----------------------------------------------------------------------------
AxisAlignedBoundingBox::AxisAlignedBoundingBox()
    :   mMinCorner(numeric_limits<double>::max()),
        mMaxCorner(-numeric_limits<double>::max())
{}

//-----------------------------------------------------------------------------
void AxisAlignedBoundingBox::add(const AxisAlignedBoundingBox &iOther)
{
	if (iOther.isValid())
	{
		addPoint(iOther.mMinCorner);
        addPoint(iOther.mMaxCorner);
	}
}

//-----------------------------------------------------------------------------
void AxisAlignedBoundingBox::addPoint(const Math::Vector3& iP)
{
    mMinCorner.set(
        std::min(mMinCorner.x(), iP.x()),
        std::min(mMinCorner.y(), iP.y()),
        std::min(mMinCorner.z(), iP.z()));

    //set the maximum point
    mMaxCorner.set(
        std::max(mMaxCorner.x(), iP.x()),
        std::max(mMaxCorner.y(), iP.y()),
        std::max(mMaxCorner.z(), iP.z()));
}

//-----------------------------------------------------------------------------
bool AxisAlignedBoundingBox::contains(const Math::Vector3& iPoint, bool iProper /*= false*/) const
{
    if(iProper)
        return ( (mMinCorner.x()<iPoint.x()) && (iPoint.x()<mMaxCorner.x())
            &&
            (mMinCorner.y()<iPoint.y()) && (iPoint.y()<mMaxCorner.y())
            &&
            (mMinCorner.z()<iPoint.z()) && (iPoint.z()<mMaxCorner.z()) );
    else
        return ( (mMinCorner.x()<=iPoint.x()) && (iPoint.x()<=mMaxCorner.x())
            &&
            (mMinCorner.y()<=iPoint.y()) && (iPoint.y()<=mMaxCorner.y())
            &&
            (mMinCorner.z()<=iPoint.z()) && (iPoint.z()<=mMaxCorner.z()) );

}

//-----------------------------------------------------------------------------
double AxisAlignedBoundingBox::getArea() const
{ 
    Math::Vector3 diag = mMaxCorner-mMinCorner;
    return (2.*diag.x()*diag.y()) + (2.*diag.z()*diag.y()) + (2.*diag.x()*diag.z());
}

//-----------------------------------------------------------------------------
Math::Vector3 AxisAlignedBoundingBox::getCenter() const
{ return 0.5*(mMinCorner + mMaxCorner); }

//-----------------------------------------------------------------------------
std::vector<Math::Vector3>  AxisAlignedBoundingBox::getCorners() const
{
    std::vector<Math::Vector3> corners(8, Vector3());

    corners[0] = Math::Vector3(mMinCorner.x(), mMinCorner.y(), mMinCorner.z());
    corners[1] = Math::Vector3(mMaxCorner.x(), mMinCorner.y(), mMinCorner.z());
    corners[2] = Math::Vector3(mMinCorner.x(), mMaxCorner.y(), mMinCorner.z());
    corners[3] = Math::Vector3(mMaxCorner.x(), mMaxCorner.y(), mMinCorner.z());
    corners[4] = Math::Vector3(mMinCorner.x(), mMinCorner.y(), mMaxCorner.z());
    corners[5] = Math::Vector3(mMaxCorner.x(), mMinCorner.y(), mMaxCorner.z());
    corners[6] = Math::Vector3(mMinCorner.x(), mMaxCorner.y(), mMaxCorner.z());
    corners[7] = Math::Vector3(mMaxCorner.x(), mMaxCorner.y(), mMaxCorner.z());

    return corners;
}

//-----------------------------------------------------------------------------
const Math::Vector3& AxisAlignedBoundingBox::getMaxCorner() const
{
    return mMaxCorner;
}

//-----------------------------------------------------------------------------
const Math::Vector3& AxisAlignedBoundingBox::getMinCorner() const
{
    return mMinCorner;
}

//-----------------------------------------------------------------------------
double AxisAlignedBoundingBox::getVolume() const
{ 
    Math::Vector3 diag = mMaxCorner-mMinCorner;
    return diag.x()*diag.y()*diag.z(); 
}

//-----------------------------------------------------------------------------
Vector3 AxisAlignedBoundingBox::getSize() const
{
    return mMaxCorner - mMinCorner;
}

//-----------------------------------------------------------------------------
bool AxisAlignedBoundingBox::isValid() const
{
    Vector3 maxMinusMin = mMaxCorner-mMinCorner;

    // When default constructed, the box appear has valid
    // because the above substraction give -infinity...
    // for that reason, we check for infinity
    //
    return !std::isinf(maxMinusMin.x()) && maxMinusMin.normSquared() > 0.;
}

//-----------------------------------------------------------------------------
bool AxisAlignedBoundingBox::intersects(const AxisAlignedBoundingBox &iB) const
{
  return (mMinCorner.x() <= iB.mMaxCorner.x() && mMaxCorner.x() >= iB.mMinCorner.x()) &&
         (mMinCorner.y() <= iB.mMaxCorner.y() && mMaxCorner.y() >= iB.mMinCorner.y()) &&
         (mMinCorner.z() <= iB.mMaxCorner.z() && mMaxCorner.z() >= iB.mMinCorner.z());
}

//-----------------------------------------------------------------------------
Mesh AxisAlignedBoundingBox::makeMesh() const
{ 
    Mesh mesh;

    // 8 vertices
    // 6 faces
    // 4 vertices per face
    mesh.setNumberOfVerticesPerFace(4);

    //-- grab references to guts of mesh
    vector<Mesh::VertexData>& vertices = mesh.getVerticesRef();

    const Vector3 bl( mMinCorner.x(), mMinCorner.y(), mMinCorner.z() );
    const Vector3 tr(mMaxCorner.x(), mMaxCorner.y(), mMaxCorner.z());

    vertices.resize(8, Mesh::VertexData());
    //-Z
    vertices[0].mVertex = Vector3(bl.x(), bl.y(), bl.z());
    vertices[1].mVertex = Vector3(tr.x(), bl.y(), bl.z());
    vertices[2].mVertex = Vector3(tr.x(), tr.y(), bl.z());
    vertices[3].mVertex = Vector3(bl.x(), tr.y(), bl.z());

    //+Z
    vertices[4].mVertex = Vector3(bl.x(), bl.y(), tr.z());
    vertices[5].mVertex = Vector3(tr.x(), bl.y(), tr.z());
    vertices[6].mVertex = Vector3(tr.x(), tr.y(), tr.z());
    vertices[7].mVertex = Vector3(bl.x(), tr.y(), tr.z());

    // 6 faces with 4 vertex each... so 24 entries
    mesh.makeFaceB({ 0, 1, 2, 3 }); //0, 1, 2, 3
    mesh.makeFaceB({ 4, 5, 6 ,7 }); //4, 5, 6, 7
    mesh.makeFaceB({ 5, 1, 2, 6 }); //8, 9, 10, 11  
    mesh.makeFaceB({ 0, 4, 7, 3 }); //12, 13, 14, 15
    mesh.makeFaceB({ 6, 2, 3, 7 }); //16, 17, 18, 19  
    mesh.makeFaceB({ 0, 1, 5, 4 }); //20 21 22 23

    mesh.triangulate();

    return mesh;
}

//-----------------------------------------------------------------------------
const bool AxisAlignedBoundingBox::operator == (const AxisAlignedBoundingBox& aabb) const
{
    return (mMinCorner == aabb.mMinCorner) && (mMaxCorner == aabb.mMaxCorner);
}

//-----------------------------------------------------------------------------
const bool AxisAlignedBoundingBox::operator != (const AxisAlignedBoundingBox& aabb) const
{
    return !(*this == aabb);
}

//-----------------------------------------------------------------------------
const bool AxisAlignedBoundingBox::operator <= (const AxisAlignedBoundingBox& aabb) const
{
    return aabb.contains(mMinCorner) && aabb.contains(mMaxCorner);
}
//-----------------------------------------------------------------------------
const bool AxisAlignedBoundingBox::operator < (const AxisAlignedBoundingBox& aabb) const
{
    return aabb.contains(mMinCorner,true) && aabb.contains(mMaxCorner, true);
}

//-----------------------------------------------------------------------------
void AxisAlignedBoundingBox::reset()
{
    mMinCorner = Vector3(numeric_limits<double>::max());
    mMaxCorner = Vector3 (-numeric_limits<double>::max());
}

//-----------------------------------------------------------------------------
void AxisAlignedBoundingBox::set(Math::Vector3 iMin, Math::Vector3 iMax)
{
    mMinCorner = iMin;
    mMaxCorner = iMax;
}

//-----------------------------------------------------------------------------
AxisAlignedBoundingBox& AxisAlignedBoundingBox::transform(const Math::Matrix4& iM)
{
    reset();
    addPoint( (iM * Vector4(mMinCorner.x(), mMinCorner.y(), mMinCorner.z(), 1.0)).xyz());
    addPoint( (iM * Vector4(mMaxCorner.x(), mMinCorner.y(), mMinCorner.z(), 1.0)).xyz());
    addPoint( (iM * Vector4(mMinCorner.x(), mMaxCorner.y(), mMinCorner.z(), 1.0)).xyz());
    addPoint( (iM * Vector4(mMaxCorner.x(), mMaxCorner.y(), mMinCorner.z(), 1.0)).xyz());
    addPoint( (iM * Vector4(mMinCorner.x(), mMinCorner.y(), mMaxCorner.z(), 1.0)).xyz());
    addPoint( (iM * Vector4(mMaxCorner.x(), mMinCorner.y(), mMaxCorner.z(), 1.0)).xyz());
    addPoint( (iM * Vector4(mMinCorner.x(), mMaxCorner.y(), mMaxCorner.z(), 1.0)).xyz());
    addPoint( (iM * Vector4(mMaxCorner.x(), mMaxCorner.y(), mMaxCorner.z(), 1.0)).xyz());
    return *this;
}

//-----------------------------------------------------------------------------
AxisAlignedBoundingBox AxisAlignedBoundingBox::transformed(const Math::Matrix4& iM) const
{
    AxisAlignedBoundingBox bb;

    bb.addPoint( (iM * Vector4(mMinCorner.x(), mMinCorner.y(), mMinCorner.z(), 1.0)).xyz());
    bb.addPoint( (iM * Vector4(mMaxCorner.x(), mMinCorner.y(), mMinCorner.z(), 1.0)).xyz());
    bb.addPoint( (iM * Vector4(mMinCorner.x(), mMaxCorner.y(), mMinCorner.z(), 1.0)).xyz());
    bb.addPoint( (iM * Vector4(mMaxCorner.x(), mMaxCorner.y(), mMinCorner.z(), 1.0)).xyz());
    bb.addPoint( (iM * Vector4(mMinCorner.x(), mMinCorner.y(), mMaxCorner.z(), 1.0)).xyz());
    bb.addPoint( (iM * Vector4(mMaxCorner.x(), mMinCorner.y(), mMaxCorner.z(), 1.0)).xyz());
    bb.addPoint( (iM * Vector4(mMinCorner.x(), mMaxCorner.y(), mMaxCorner.z(), 1.0)).xyz());
    bb.addPoint( (iM * Vector4(mMaxCorner.x(), mMaxCorner.y(), mMaxCorner.z(), 1.0)).xyz());

    return bb;
}

