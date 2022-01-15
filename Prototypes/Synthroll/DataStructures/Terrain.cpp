
#include "Core/Logger.h"
#include "DataStructures/Terrain.h"

using namespace Realisim;
using namespace Synthroll;

// statics...
Geometry::LineSegment Terrain::mDummySegment;

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
// adds a segment and returns the index of the segment
int Terrain::addSegment(const Geometry::LineSegment& iS)
{
    mSegments.push_back(iS);
    const int segmentSize = getNumberOfSegments();
    mModelNodes.resize(segmentSize);
    mSegmentIsDirty.resize(segmentSize);

    setSegmentAsDirty(segmentSize - 1, true);
    
    return segmentSize - 1;
}

//---------------------------------------------------------------------------------------------------------------------
void Terrain::clear()
{
    mSegments.clear();
    mModelNodes.clear();
    mSegmentIsDirty.clear();
    mIsDirty = false;
}

//---------------------------------------------------------------------------------------------------------------------
const Reactor::ModelNode* Terrain::getModelNode(int iIndex) const {
    const Reactor::ModelNode* r = nullptr;
    if (iIndex >= 0 && iIndex < getNumberOfSegments())
    {
        r = mModelNodes[iIndex];
    }

    return r;
}

//---------------------------------------------------------------------------------------------------------------------
Reactor::ModelNode* Terrain::getModelNodePtr(int iIndex){
    
    return const_cast<Reactor::ModelNode*>(
        const_cast<const Terrain*>(this)->getModelNode(iIndex));
}

//---------------------------------------------------------------------------------------------------------------------
int Terrain::getNumberOfSegments() const
{
    return (int)mSegments.size();
}

//---------------------------------------------------------------------------------------------------------------------
const Geometry::LineSegment& Terrain::getSegment(int iIndex) const
{
    const Geometry::LineSegment* r = &mDummySegment;
    if (iIndex >= 0 && iIndex < getNumberOfSegments())
    {
        r = &mSegments[iIndex];
    }

    return *r;
}

//---------------------------------------------------------------------------------------------------------------------
Geometry::LineSegment& Terrain::getSegmentRef(int iIndex)
{
    return const_cast<Geometry::LineSegment&>( const_cast<const Terrain*>(this)->getSegment(iIndex) );
}

//---------------------------------------------------------------------------------------------------------------------
bool Terrain::isSegmentDirty(int iIndex) const
{
    bool r = false;
    if (iIndex >= 0 && iIndex < getNumberOfSegments())
    {
        r = mSegmentIsDirty[iIndex];
    }
    return r;
}

//---------------------------------------------------------------------------------------------------------------------
void Terrain::removeSegment(int iIndex)
{
    if (iIndex >= 0 && iIndex < getNumberOfSegments())
    {
        mSegments.erase(mSegments.begin() + iIndex);
        mModelNodes.erase(mModelNodes.begin() + iIndex);
    }

    // mark all segment as dirty
    for (int i = 0; i < getNumberOfSegments(); ++i)
    { setSegmentAsDirty(i, true); }
}

//---------------------------------------------------------------------------------------------------------------------
void Terrain::setModelNode(int iIndex, Reactor::ModelNode* ipModel)
{
    if (iIndex >= 0 && iIndex < getNumberOfSegments())
    {
        mModelNodes[iIndex] = ipModel;
    }
    else {
        LOG_TRACE_ERROR(Core::Logger::llNormal, "Trying to add model node to an invalid segment index [%d]", iIndex);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void Terrain::setSegmentAsDirty(int iIndex, bool iV)
{
    if (iIndex >= 0 && iIndex < getNumberOfSegments())
    {
        mSegmentIsDirty[iIndex] = iV;
    }

    mIsDirty = false;
    for (auto d : mSegmentIsDirty) {
        mIsDirty |= d;
    }
}