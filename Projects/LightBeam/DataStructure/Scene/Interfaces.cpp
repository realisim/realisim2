
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
//---IRenderable
//-----------------------------------------------------------------------------
IRenderable::IRenderable() : mpMaterial(make_shared<Material>())
{}

//-----------------------------------------------------------------------------
IRenderable::~IRenderable()
{}

//-----------------------------------------------------------------------------
const std::shared_ptr<Material> IRenderable::getMaterial() const
{ return mpMaterial; }

//-----------------------------------------------------------------------------
bool IRenderable::intersects(const Geometry::Line& iRay) const
{ 
    UNUSED(iRay);
    return false;
}

//-----------------------------------------------------------------------------
bool IRenderable::intersect(const Geometry::Line& iRay, IntersectionResult* opResult) const
{ 
    UNUSED(iRay);
    UNUSED(opResult);
    return false;
}

//-----------------------------------------------------------------------------
void IRenderable::setMaterial(std::shared_ptr<Material> ipMaterial)
{ mpMaterial = ipMaterial; }

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

