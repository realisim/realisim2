
#include "Interfaces.h"
#include "IntersectionResult.h"

using namespace Realisim;
    using namespace LightBeam;
using namespace std;


//-----------------------------------------------------------------------------
//--- ISceneNode
//-----------------------------------------------------------------------------
uint32_t ISceneNode::mIdCounter = 0;

//-----------------------------------------------------------------------------
ISceneNode::ISceneNode() : mId(++mIdCounter),
    mType(ntSceneNode)
{}

//-----------------------------------------------------------------------------
ISceneNode::ISceneNode(NodeType iNodeType) : mId(++mIdCounter),
    mType(iNodeType)
{}

//-----------------------------------------------------------------------------
ISceneNode::~ISceneNode()
{}

//-----------------------------------------------------------------------------
uint32_t ISceneNode::getId() const
{ return mId;}

//-----------------------------------------------------------------------------
ISceneNode::NodeType ISceneNode::getNodeType() const
{ return mType; }

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
{ return false; }

//-----------------------------------------------------------------------------
bool IRenderable::intersect(const Geometry::Line& iRay, IntersectionResult* opResult) const
{ return false; }

//-----------------------------------------------------------------------------
void IRenderable::setMaterial(std::shared_ptr<Material> ipMaterial)
{ mpMaterial = ipMaterial; }

