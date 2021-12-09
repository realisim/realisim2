
#include "DataStructures/Scene/Scene.h"

using namespace Realisim;
    using namespace Reactor;
    using namespace ThreeD;

//---------------------------------------------------------------------------------------------------------------------
Scene::Scene() :
    mpRoot(nullptr),
    mpImageLibrary(nullptr),
    mpMaterialLibrary(nullptr)
{
    makeNewScene();
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
    makeNewScene();    
}

//---------------------------------------------------------------------------------------------------------------------
void Scene::makeNewScene()
{
    mpRoot = new SceneNode();
    mpRoot->setName("Root");

    mpImageLibrary = new SceneNode();
    mpImageLibrary->setName("ImageLibrary");

    mpMaterialLibrary = new SceneNode();
    mpMaterialLibrary->setName("MaterialLibrary");

    mpRoot->addChild(mpImageLibrary);
    mpRoot->addChild(mpMaterialLibrary);
}