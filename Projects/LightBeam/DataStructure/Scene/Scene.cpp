
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
        case ISceneNode::ntLight:
        {
            shared_ptr<ILightNode> c = dynamic_pointer_cast<ILightNode>(ipNode);
            if(c)
            {
                mLights.push_back(c);
            }
        }break;
        default: assert(0); break;
    }
}

//-----------------------------------------------------------------------------
const std::vector< std::shared_ptr<ILightNode> >& Scene::getLights() const
{
    return mLights;
}

//-----------------------------------------------------------------------------
const std::vector< std::shared_ptr<IRenderable> >& Scene::getRenderables() const
{
    return mRenderables;
}