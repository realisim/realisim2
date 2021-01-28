
#include "Core/Unused.h"
#include "Interfaces.h"
#include "IntersectionResult.h"
#include "Math/CommonMath.h"

using namespace Realisim;
    using namespace LightBeam;
    using namespace ThreeD;
using namespace std;


//-----------------------------------------------------------------------------
//--- ISceneNode
//-----------------------------------------------------------------------------
uint32_t ISceneNode::mIdCounter = 0;

//-----------------------------------------------------------------------------
ISceneNode::ISceneNode() : mId(++mIdCounter),
    mType(ntSceneNode),
    mName("unnamed")
{}

//-----------------------------------------------------------------------------
ISceneNode::ISceneNode(NodeType iNodeType) : mId(++mIdCounter),
    mType(iNodeType)
{}

//-----------------------------------------------------------------------------
ISceneNode::~ISceneNode()
{}

//-----------------------------------------------------------------------------
const Geometry::AxisAlignedBoundingBox& ISceneNode::getAxisAlignedBoundingBox() const
{
    return mAABB;
}

//-----------------------------------------------------------------------------
uint32_t ISceneNode::getId() const
{ return mId;}

//-----------------------------------------------------------------------------
std::string ISceneNode::getName() const
{
    return mName;
}

//-----------------------------------------------------------------------------
ISceneNode::NodeType ISceneNode::getNodeType() const
{ return mType; }

//-----------------------------------------------------------------------------
void ISceneNode::setAxisAlignedBoundingBox(const Geometry::AxisAlignedBoundingBox& iAABB)
{
    mAABB = iAABB;
}

//-----------------------------------------------------------------------------
void ISceneNode::setName(const std::string& iV)
{
    mName = iV;
}

//-----------------------------------------------------------------------------
//---ILight
//-----------------------------------------------------------------------------
ILightNode::ILightNode()
{}

//-----------------------------------------------------------------------------
ILightNode::~ILightNode()
{}

//-----------------------------------------------------------------------------
double ILightNode::computeLi() const
{ return 1.0; }

//----------------------------------------------------------------------
const Light& ILightNode::getLight() const
{ return mLight; }

//----------------------------------------------------------------------
void ILightNode::setLight(const Light& iL)
{ mLight = iL; }

