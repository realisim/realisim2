
#include "Core/Unused.h"
#include "IRenderable.h"

using namespace Realisim;
    using namespace LightBeam;
    using namespace ThreeD;
using namespace std;

//-----------------------------------------------------------------------------
//---IRenderable
//-----------------------------------------------------------------------------
IRenderable::IRenderable() : 
    mpMaterialNode(make_shared<MaterialNode>())
{}

//-----------------------------------------------------------------------------
IRenderable::~IRenderable()
{}

//-----------------------------------------------------------------------------
const std::shared_ptr<MaterialNode> IRenderable::getMaterialNode() const
{
    return mpMaterialNode;
}

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
void IRenderable::setMaterialNode(std::shared_ptr<MaterialNode> iV)
{
    mpMaterialNode = iV;
}