
#include "Core/Logger.h"
#include "DataStructures/Terrain.h"

using namespace Realisim;
    using namespace Math;
using namespace Synthroll;


//---------------------------------------------------------------------------------------------------------------------
//--- TerrainSpline
//---------------------------------------------------------------------------------------------------------------------
TerrainSpline::~TerrainSpline() {
    mSegments.clear();
    m2dPolygons.clear();
}

// statics...
TerrainSpline Terrain::mDummySpline;
Geometry::LineSegment Terrain::mDummySegment;
Polygon2d Terrain::mDummyPolygon;
//---------------------------------------------------------------------------------------------------------------------
//--- Terrain
//---------------------------------------------------------------------------------------------------------------------
Terrain::Terrain() :
    mIsDirty(false),
    mStartPoint(-5, 10.0),
    mEndRect(Math::Vector2(100.0, 100.0), 5.0, 5.0)
{}

//---------------------------------------------------------------------------------------------------------------------
Terrain::~Terrain()
{
    clear();
}

//---------------------------------------------------------------------------------------------------------------------
// add a new empty spline and returns the index of the spline.
//
int Terrain::addSpline()
{
    mSplines.push_back(new TerrainSpline());
    const int size = getNumberOfSplines();
    mSplineModelNodes.resize(size);
    mSplineIsDirty.resize(size);
    setSplineAsDirty(size - 1, true);

    return size - 1;
}

//---------------------------------------------------------------------------------------------------------------------
// adds a segment and returns the index of the segment
int Terrain::addSegment(int iSplineIndex, const Geometry::LineSegment& iS)
{
    TerrainSpline& ts = getSplineRef(iSplineIndex);
    ts.mSegments.push_back(Geometry::LineSegment(iS));
    setSplineAsDirty(iSplineIndex, true);    
    
    return (int)ts.mSegments.size() - 1;
}

//---------------------------------------------------------------------------------------------------------------------
void Terrain::clear()
{
    // delete all terrainSpline
    for (auto pSpline : mSplines)
        delete pSpline;
    mSplines.clear();
    mSplineModelNodes.clear();
    mSplineIsDirty.clear();
    mIsDirty = false;
}

//---------------------------------------------------------------------------------------------------------------------
void Terrain::computePolygons(int iSplineIndex)
{
    TerrainSpline& ts = getSplineRef(iSplineIndex);
    ts.m2dPolygons.clear();

    int polygonIndex = 0;
    const int numSegments = getNumberOfSegments(iSplineIndex);
    for (int i = 0; i < numSegments; ++i) {

        const Geometry::LineSegment& ls = ts.mSegments[i];
        // a segment mus have a lenght to become a polygon...
        if (ls.lengthSquared() > 0.01)
        {
            Vector3 perpendicularDir = (ls.mP1 - ls.mP0).cross(Vector3(0.0, 0.0, 1.0));
            perpendicularDir.normalize();
            Vector3 halfPerpendicular = perpendicularDir * 0.5;

            // use the previous polygon last 2 points instead of creating new ones
            Vector2 v0 = (ls.mP0 - halfPerpendicular).xy();
            Vector2 v1 = (ls.mP0 + halfPerpendicular).xy();
            if (polygonIndex > 0)
            {
                const Polygon2d& poly = ts.m2dPolygons[polygonIndex - 1];
                v0 = poly.getVertices()[3];
                v1 = poly.getVertices()[2];
            }
            const Vector2 v2 = (ls.mP1 + halfPerpendicular).xy();
            const Vector2 v3 = (ls.mP1 - halfPerpendicular).xy();

            Polygon2d p;
            p.setVertices({ v0, v1, v2, v3 });
            ts.m2dPolygons.push_back(p);
            polygonIndex++;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
int Terrain::getNumberOf2dPolygons(int iSplineIndex) const
{
    const TerrainSpline& ts = getSpline(iSplineIndex);
    return (int)ts.m2dPolygons.size();
}

//---------------------------------------------------------------------------------------------------------------------
int Terrain::getNumberOfSegments(int iSplineIndex) const
{
    const TerrainSpline& ts = getSpline(iSplineIndex);
    return (int)ts.mSegments.size();
}

//---------------------------------------------------------------------------------------------------------------------
const Polygon2d& Terrain::get2dPolygon(int iSplineIndex, int iIndex) const
{
    const TerrainSpline& ts = getSpline(iSplineIndex);
    const Polygon2d* r = &mDummyPolygon;
    if (iIndex >= 0 && iIndex < getNumberOf2dPolygons(iSplineIndex))
    {
        r = &ts.m2dPolygons[iIndex];
    }

    return *r;
}

//---------------------------------------------------------------------------------------------------------------------
const Geometry::LineSegment& Terrain::getSegment(int iSplineIndex, int iIndex) const
{
    const TerrainSpline& ts = getSpline(iSplineIndex);
    const Geometry::LineSegment* r = &mDummySegment;
    if (iIndex >= 0 && iIndex < getNumberOfSegments(iSplineIndex))
    {
        r = &ts.mSegments[iIndex];
    }

    return *r;
}

//---------------------------------------------------------------------------------------------------------------------
Geometry::LineSegment& Terrain::getSegmentRef(int iSplineIndex, int iIndex)
{
    return const_cast<Geometry::LineSegment&>( const_cast<const Terrain*>(this)->getSegment(iSplineIndex, iIndex) );
}

//---------------------------------------------------------------------------------------------------------------------
const TerrainSpline& Terrain::getSpline(int iSplineIndex) const
{
    const TerrainSpline* r = &mDummySpline;
    if (iSplineIndex >= 0 && iSplineIndex < getNumberOfSplines()) {
        r = mSplines[iSplineIndex];
    }
    return *r;
}

//---------------------------------------------------------------------------------------------------------------------
TerrainSpline& Terrain::getSplineRef(int iSplineIndex)
{
    return const_cast<TerrainSpline&>(const_cast<const Terrain*>(this)->getSpline(iSplineIndex));
}

//---------------------------------------------------------------------------------------------------------------------
const Reactor::ModelNode* Terrain::getSplineModelNode(int iIndex) const {
    const Reactor::ModelNode* r = nullptr;
    if (iIndex >= 0 && iIndex < getNumberOfSplines())
    {
        r = mSplineModelNodes[iIndex];
    }

    return r;
}

//---------------------------------------------------------------------------------------------------------------------
Reactor::ModelNode* Terrain::getSplineModelNodePtr(int iIndex) {

    return const_cast<Reactor::ModelNode*>(
        const_cast<const Terrain*>(this)->getSplineModelNode(iIndex));
}


//---------------------------------------------------------------------------------------------------------------------
bool Terrain::isSplineDirty(int iSplineIndex) const
{
    bool r = false;
    if (iSplineIndex >= 0 && iSplineIndex < getNumberOfSplines())
    {
        r = mSplineIsDirty[iSplineIndex];
    }
    return r;
}

//---------------------------------------------------------------------------------------------------------------------
// remove a spline and returns the index of the last spline, -1 means no more spline
int Terrain::removeSpline(int iSplineIndex) {    

    if (iSplineIndex >= 0 && iSplineIndex < getNumberOfSplines()) {
        mSplines.erase(mSplines.begin() + iSplineIndex);
        mSplineModelNodes.erase(mSplineModelNodes.begin() + iSplineIndex);
    }

    // mark all spline as dirty
    for (int i = 0; i < getNumberOfSplines(); ++i) {
        setSplineAsDirty(i, true);
    }

    return (int)mSplines.size() - 1;
}

//---------------------------------------------------------------------------------------------------------------------
int Terrain::removeSegment(int iSplineIndex, int iIndex)
{
    TerrainSpline& ts = getSplineRef(iSplineIndex);
    if (iIndex >= 0 && iIndex < getNumberOfSegments(iSplineIndex))
    {
        ts.mSegments.erase(ts.mSegments.begin() + iIndex);

        // mark spline as dirty
        setSplineAsDirty(iSplineIndex, true);
    }
    return (int)ts.mSegments.size() - 1;
}

//---------------------------------------------------------------------------------------------------------------------
void Terrain::setSplineModelNode(int iIndex, Reactor::ModelNode* ipModel)
{
    if (iIndex >= 0 && iIndex < getNumberOfSplines())
    {
        mSplineModelNodes[iIndex] = ipModel;
    }
    else {
        LOG_TRACE_ERROR(Core::Logger::llNormal, "Trying to add model node to an invalid segment index [%d]", iIndex);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void Terrain::setSplineAsDirty(int iIndex, bool iV)
{
    if (iIndex >= 0 && iIndex < getNumberOfSplines())
    {
        mSplineIsDirty[iIndex] = iV;
        if(iV)
            computePolygons(iIndex);
    }

    mIsDirty = false;
    for (auto d : mSplineIsDirty) {
        mIsDirty |= d;
    }
}