
#include <cassert>
#include "Scene.h"
#include "IntersectionResult.h"

using namespace Realisim;
    using namespace LightBeam;
using namespace std;

//-----------------------------------------------------------------------------
Scene::Scene()
{}

//-----------------------------------------------------------------------------
Scene::~Scene()
{
    mNodes.clear();
    mRenderables.clear();
}

//-----------------------------------------------------------------------------
void Scene::addNode(std::shared_ptr<ISceneNode> ipNode)
{
    mNodes.push_back(ipNode);
    
    switch(ipNode->getNodeType())
    {
        case ISceneNode::ntSceneNode: break;
        case ISceneNode::ntRenderable:
        {
            shared_ptr<IRenderable> c = dynamic_pointer_cast<IRenderable>(ipNode);
            if(c)
            {
                mRenderables.push_back(c);
            }
        } break;
            //case ISceneNode::ntLight: break;
        default: assert(0); break;
    }
}

//-----------------------------------------------------------------------------
const std::vector< std::shared_ptr<IRenderable> >& Scene::getRenderables() const
{
    return mRenderables;
}

////-----------------------------------------------------------------------------
//bool Scene::intersects(const Geometry::Line& iRay) const
//{
//    bool r = false;
//    for(size_t i = 0; i < mRenderables.size() && !r; ++i)
//    {
//        r |= mRenderables[i]->intersects(iRay);
//    }
//    return r;
//}
//
////-----------------------------------------------------------------------------
//bool Scene::intersect(const Geometry::Line& iRay, IntersectionResult* opResult) const
//{
//    IntersectionResult irTemp;
//    IntersectionResult ir;
//    
//    bool r = false;
//    for(size_t i = 0; i < mRenderables.size(); ++i)
//    {
//        r |= mRenderables[i]->intersect(iRay, &irTemp);
//    }
//    
//    if(opResult)
//    {
//        (*opResult).mPointInWorldSpace = irTemp.mPointInWorldSpace;
//        (*opResult).mNormal = irTemp.mNormal;
//    }
//    return r;
//}
