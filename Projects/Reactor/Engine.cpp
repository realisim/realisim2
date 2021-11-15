
#include "Engine.h"
#include "Systems/Renderer/Renderer.h"

using namespace Realisim;
    using namespace Reactor;

//---------------------------------------------------------------------------------------------------------------------
void Engine::createCoreSystems() {

    // create and assign systems to the hub
    // renderer
    Renderer* pRenderer = new Renderer(&mBroker, &mHub);

    mSystems.push_back(pRenderer);

    // assign to hub
    mHub.mpRenderer = pRenderer;
}

//---------------------------------------------------------------------------------------------------------------------
bool Engine::init()
{
    bool r = true;

    createCoreSystems();

    for (auto s : mSystems)
    { r &= s->preInit(); }

    for (auto s : mSystems)
    { r &= s->init(); }

    for (auto s : mSystems)
    { r &= s->postInit(); }

    return r;
}

//---------------------------------------------------------------------------------------------------------------------
void Engine::update()
{
    for (auto s : mSystems)
        s->preUpdate();

    for (auto s : mSystems)
        s->update();

    for (auto s : mSystems)
        s->postUpdate();
}

////---------------------------------------------------------------------------------------------------------------------
//void Engine::start()
//{}
//
////---------------------------------------------------------------------------------------------------------------------
//void Engine::stop()
//{}

//---------------------------------------------------------------------------------------------------------------------
void Engine::terminate()
{
    for (auto s : mSystems)
    { s->preTerminate(); }

    for (auto s : mSystems)
    { s->terminate(); }

    for (auto s : mSystems)
    { s->postTerminate(); }

    // delete all systems and clear hub
    for (auto s : mSystems)
    {
        delete s;
        s = nullptr;
    }
    mSystems.clear();
    mHub.clear();
}