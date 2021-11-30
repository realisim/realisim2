
#include "DataStructures/Scene/Scene.h"

using namespace Realisim;
    using namespace Reactor;
    using namespace ThreeD;

//---------------------------------------------------------------------------------------------------------------------
Scene::Scene() :
    mpRoot(new SceneNode())
{
    mpRoot->setName("Root");
}

//---------------------------------------------------------------------------------------------------------------------
Scene::~Scene()
{
    delete mpRoot;
    mpRoot = nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
void Scene::clear() {
    delete mpRoot;
    mpRoot = new SceneNode();
    mpRoot->setName("Root");
}