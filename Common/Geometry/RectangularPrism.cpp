#include <algorithm>
#include <cassert>
#include "RectangularPrism.h"
#include <vector>

using namespace Realisim;
    using namespace Geometry;
    using namespace Math;
using namespace std;

//------------------------------------------------------------------------------
RectangularPrism::RectangularPrism() :
    mIsValid(false),
    mBottomLeft(),
    mTopRight()
{}

//------------------------------------------------------------------------------
RectangularPrism::RectangularPrism(const Vector3& iBottomLeft,
    const Vector3& iTopRight) :
    mBottomLeft(iBottomLeft),
    mTopRight(iTopRight)
{
    validate(); 
}

//------------------------------------------------------------------------------
RectangularPrism::RectangularPrism(const Vector3& iBottomLeft, 
    double iWidth,
    double iHeight,
    double iDepth) :
    mBottomLeft(iBottomLeft),
    mTopRight(mBottomLeft + Vector3(iWidth, iHeight, iDepth))
{
    validate();
}

//------------------------------------------------------------------------------
RectangularPrism::~RectangularPrism()
{}

//------------------------------------------------------------------------------
// returns true if the point iPos is inside or on the edge of the rectangularPrism.
// otherwise, returns false.
//
// If proper is true, it returns true only if the point is inside the rectangularPrism.
//
bool RectangularPrism::contains( const Vector3& iPos, bool iProper /*=false*/) const
{
    bool r = false;
    if (iProper)
    {
        r = iPos.x() > getLeft() &&
            iPos.x() < getRight() &&
            iPos.y() > getBottom() &&
            iPos.y() < getTop() &&
            iPos.z() < getNear() &&
            iPos.z() > getFar();
    }
    else
    {
        r = iPos.x() >= getLeft() &&
            iPos.x() <= getRight() &&
            iPos.y() >= getBottom() &&
            iPos.y() <= getTop() &&
            iPos.z() <= getNear() &&
            iPos.z() >= getFar();
    }
    return r;
}

//------------------------------------------------------------------------------
// returns true if the rectangularPrism iRect is inside or on the edge of the
// current rectangularPrism.
//
// If proper is true, it returns true only if the rectangularPrism is completely inside 
// the rectangularPrism (not touching the edge)
bool RectangularPrism::contains(const RectangularPrism& iRect, bool iProper /*= false*/) const
{
    return contains(iRect.getNearBottomLeft(), iProper) &&
        contains(iRect.getFarTopRight(), iProper);
}

//------------------------------------------------------------------------------
double RectangularPrism::getBottom() const
{ return mBottomLeft.y(); }

//------------------------------------------------------------------------------
Vector3 RectangularPrism::getCenter() const
{
    return mBottomLeft + getSize() / 2.0;
}

//------------------------------------------------------------------------------
double RectangularPrism::getDepth() const
{ return fabs(mTopRight.z() - mBottomLeft.z()); }

//------------------------------------------------------------------------------
double RectangularPrism::getFar() const
{ return mBottomLeft.z(); }

//------------------------------------------------------------------------------
Math::Vector3 RectangularPrism::getFarBottomLeft() const
{ return Vector3(mBottomLeft.x(), mBottomLeft.y(), mBottomLeft.z()); }

//------------------------------------------------------------------------------
Math::Vector3 RectangularPrism::getFarBottomRight() const
{ return Vector3(mTopRight.x(), mBottomLeft.y(), mBottomLeft.z()); }

//------------------------------------------------------------------------------
Math::Vector3 RectangularPrism::getFarTopLeft() const
{ return Vector3(mBottomLeft.x(), mTopRight.y(), mBottomLeft.z()); }

//------------------------------------------------------------------------------
Math::Vector3 RectangularPrism::getFarTopRight() const
{ return Vector3(mTopRight.x(), mTopRight.y(), mBottomLeft.z()); }


//------------------------------------------------------------------------------
double RectangularPrism::getHeight() const
{ return fabs(mTopRight.y() - mBottomLeft.y()); }

//------------------------------------------------------------------------------
double RectangularPrism::getLeft() const
{ return mBottomLeft.x(); }

//------------------------------------------------------------------------------
double RectangularPrism::getNear() const
{ return mTopRight.z(); }

//------------------------------------------------------------------------------
Math::Vector3 RectangularPrism::getNearBottomLeft() const
{ return Vector3(mBottomLeft.x(), mBottomLeft.y(), mTopRight.z()); }

//------------------------------------------------------------------------------
Math::Vector3 RectangularPrism::getNearBottomRight() const
{ return Vector3(mTopRight.x(), mBottomLeft.y(), mTopRight.z()); }

//------------------------------------------------------------------------------
Math::Vector3 RectangularPrism::getNearTopLeft() const
{ return Vector3(mBottomLeft.x(), mTopRight.y(), mTopRight.z()); }

//------------------------------------------------------------------------------
Math::Vector3 RectangularPrism::getNearTopRight() const
{ return Vector3(mTopRight.x(), mTopRight.y(), mTopRight.z()); }

//------------------------------------------------------------------------------
// Returns the point iIndex, obviously, iIndex cannot be greater than 7 since
// a rectangularPrism has 8 points...
//
// The points are given in counter clockwise order starting from bottomLeft.
//
Math::Vector3 RectangularPrism::getPoint(int iIndex) const
{
    Vector3 r;
    assert(iIndex <= 7);
    switch (iIndex)
    {
    case piNearBottomLeft: r = getNearBottomLeft(); break;
    case piNearBottomRight: r = getNearBottomRight(); break;
    case piNearTopRight: r = getNearTopRight(); break;
    case piNearTopLeft: r = getNearTopLeft(); break;
    case piFarBottomLeft: r = getFarBottomLeft(); break;
    case piFarBottomRight: r = getFarBottomRight(); break;
    case piFarTopRight: r = getFarTopRight(); break;
    case piFarTopLeft: r = getFarTopLeft(); break;
    default: break;
    }
    return r;
}

//------------------------------------------------------------------------------
vector<Vector3> RectangularPrism::getPoints() const
{
    vector<Vector3> p = {getNearBottomLeft(),
        getNearBottomRight(),
        getNearTopRight(),
        getNearTopLeft(),
        getFarBottomLeft(),
        getFarBottomRight(),
        getFarTopRight(),
        getFarTopLeft() };
    p.shrink_to_fit();

    return p;
}

//------------------------------------------------------------------------------
double RectangularPrism::getRight() const
{ return mTopRight.x(); }

//------------------------------------------------------------------------------
Vector3 RectangularPrism::getSize() const
{ return mTopRight - mBottomLeft; }

//------------------------------------------------------------------------------
double RectangularPrism::getTop() const
{ return mTopRight.y(); }

//------------------------------------------------------------------------------
double RectangularPrism::getVolume() const
{
    const Vector3 s = getSize();
    return s.x() * s.y() * s.z();
}

//------------------------------------------------------------------------------
double RectangularPrism::getWidth() const
{ return fabs(mTopRight.x() - mBottomLeft.x()); }

//------------------------------------------------------------------------------
bool RectangularPrism::intersects(const RectangularPrism& iR) const
{
    bool r = false;
    if (isValid() && iR.isValid())
    {
        // we are using the minkowski method to determine if the rectangularPrisms
        // are intersecting.    

        Vector3 min(std::numeric_limits<double>::max());
        Vector3 max(-std::numeric_limits<double>::max());
        for (int j = 0; j < 8; j++)
            for (int i = 0; i < 8; i++)
            {
                Vector3 p = iR.getPoint(j) - getPoint(i);
                min.setX(std::min(min.x(), p.x()));
                min.setY(std::min(min.y(), p.y()));
                min.setZ(std::min(min.z(), p.z()));

                max.setX(std::max(max.x(), p.x()));
                max.setY(std::max(max.y(), p.y()));
                max.setZ(std::max(max.z(), p.z()));
            }

        RectangularPrism mink(min, max);
        r = mink.contains(Vector3(0.0), true);
    }
    
    return r;
}

//------------------------------------------------------------------------------
RectangularPrism RectangularPrism::intersected(const RectangularPrism& iR) const
{
    // We are the AABB overlap method to determine the resulting rectangularPrism from
    // the intersection.
    RectangularPrism r;

    if (intersects(iR))
    {
        Vector3 farBottomLeft(std::max(getLeft(), iR.getLeft()),
            std::max(getBottom(), iR.getBottom()),
            std::max(getFar(), iR.getFar()) );

        Vector3 nearTopRight(std::min(getRight(), iR.getRight()),
            std::min(getTop(), iR.getTop()),
            std::min(getNear(), iR.getNear()) );

        r.set(farBottomLeft, nearTopRight);
    }

    return r;
}

//------------------------------------------------------------------------------
bool RectangularPrism::isValid() const
{
    return mIsValid;
}

//------------------------------------------------------------------------------
Mesh RectangularPrism::makeMesh() const
{
    Mesh mesh;

    // 8 vertices
    // 6 faces
    // 4 vertices per face
    mesh.setNumberOfVerticesPerFace(4);

    //-- grab references to guts of mesh
    vector<Mesh::VertexData>& vertices = mesh.getVerticesRef();

    const Vector3 bl(getFarBottomLeft());
    const Vector3 tr(getNearTopRight());

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
    mesh.makeFace({ 0, 1, 2, 3 }, true); //0, 1, 2, 3
    mesh.makeFace({ 4, 5, 6 ,7 }, true); //4, 5, 6, 7
    mesh.makeFace({ 5, 1, 2, 6 }, true); //8, 9, 10, 11  
    mesh.makeFace({ 0, 4, 7, 3 }, true); //12, 13, 14, 15
    mesh.makeFace({ 6, 2, 3, 7 }, true); //16, 17, 18, 19  
    mesh.makeFace({ 0, 1, 5, 4 }, true); //20 21 22 23

    mesh.triangulate();

    return mesh;
}

//------------------------------------------------------------------------------
void RectangularPrism::set(const Vector3& iBottomLeft, const Vector3& iTopRight)
{
    mBottomLeft = iBottomLeft;
    mTopRight = iTopRight;
    validate();
}

//------------------------------------------------------------------------------
void RectangularPrism::set(const Vector3& iBottomLeft,
    double iWidth, double iHeight, double iDepth)
{ set(iBottomLeft, iBottomLeft + Vector3(iWidth, iHeight, iDepth)); }

//------------------------------------------------------------------------------
void RectangularPrism::set(const AxisAlignedBoundingBox& iAabb)
{
    set(iAabb.getMinCorner(),
        iAabb.getMaxCorner());
}

//------------------------------------------------------------------------------
void RectangularPrism::setBottom(double iB)
{ 
    mBottomLeft.setY(iB);
    validate();
}

//------------------------------------------------------------------------------
void RectangularPrism::setDepth(double iV)
{
    mTopRight.setZ( mBottomLeft.z() + iV);
    validate();
}

//------------------------------------------------------------------------------
void RectangularPrism::setFar(double iV)
{ 
    mBottomLeft.setZ(iV);
    validate();
}

//------------------------------------------------------------------------------
void RectangularPrism::setFarBottomLeft(const Math::Vector3& iV)
{
    mBottomLeft.setX(iV.x());
    mBottomLeft.setY(iV.y());
    mBottomLeft.setZ(iV.z());
    validate();
}

//------------------------------------------------------------------------------
void RectangularPrism::setFarBottomRight(const Math::Vector3& iV)
{
    mTopRight.setX(iV.x());
    mBottomLeft.setY(iV.y());
    mBottomLeft.setZ(iV.z());
    validate();
}

//------------------------------------------------------------------------------
void RectangularPrism::setFarTopLeft(const Math::Vector3& iV)
{
    mBottomLeft.setX(iV.x());
    mTopRight.setY(iV.y());
    mBottomLeft.setZ(iV.z());
    validate();
}

//------------------------------------------------------------------------------
void RectangularPrism::setFarTopRight(const Math::Vector3& iV)
{
    mTopRight.setX(iV.x());
    mTopRight.setY(iV.y());
    mBottomLeft.setZ(iV.z());
    validate();
}

//------------------------------------------------------------------------------
// Modifies the height of the rectangularPrism, the modification is applied to the
// top edge, the bottom edge remains unchanged.
//
void RectangularPrism::setHeight(double iV)
{
    mTopRight.setY(mBottomLeft.y() + iV);
    validate();
}

//------------------------------------------------------------------------------
void RectangularPrism::setLeft(double iV)
{
    mBottomLeft.setX(iV);
    validate();
}

//------------------------------------------------------------------------------
void RectangularPrism::setNear(double iV)
{ 
    mTopRight.setZ(iV);
    validate();
}

//------------------------------------------------------------------------------
void RectangularPrism::setNearBottomLeft(const Math::Vector3& iV)
{
    mBottomLeft.setX(iV.x());
    mBottomLeft.setY(iV.y());
    mTopRight.setZ(iV.z());
    validate();
}

//------------------------------------------------------------------------------
void RectangularPrism::setNearBottomRight(const Math::Vector3& iV)
{
    mTopRight.setX(iV.x());
    mBottomLeft.setY(iV.y());
    mTopRight.setZ(iV.z());
    validate();
}


//------------------------------------------------------------------------------
void RectangularPrism::setNearTopLeft(const Math::Vector3& iV)
{
    mBottomLeft.setX(iV.x());
    mTopRight.setY(iV.y());
    mTopRight.setZ(iV.z());
    validate();
}


//------------------------------------------------------------------------------
void RectangularPrism::setNearTopRight(const Math::Vector3& iV)
{
    mTopRight.setX(iV.x());
    mTopRight.setY(iV.y());
    mTopRight.setZ(iV.z());
    validate();
}


//------------------------------------------------------------------------------
void RectangularPrism::setRight(double iV)
{
    mTopRight.setX(iV);
    validate();
}

//------------------------------------------------------------------------------
// Modifies the size of the rectangularPrism by changing the top and right edges. The
// bottom and left edges remains unchanged
//
void RectangularPrism::setSize( const Vector3& iS)
{
    mTopRight = mBottomLeft + iS;
    validate();
}

//------------------------------------------------------------------------------
void RectangularPrism::setTop(double iV)
{
    mTopRight.setY(iV);
    validate();
}

//------------------------------------------------------------------------------
// Modifies the width of the rectangularPrism. Only the right edge is modified.
// left edge remains unmodified.
//
void RectangularPrism::setWidth(double iWidth)
{
    mTopRight.setX(mBottomLeft.x() + iWidth);
    validate();
}

//------------------------------------------------------------------------------
RectangularPrism RectangularPrism::united(const RectangularPrism& iR) const
{
    return iR;
}

//------------------------------------------------------------------------------
void RectangularPrism::validate()
{
    mIsValid = mTopRight >= mBottomLeft;
}