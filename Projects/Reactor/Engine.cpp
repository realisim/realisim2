
#include <cassert>
#include "Core/Logger.h"
#include "Engine.h"
#include "Systems/Renderer/Renderer.h"
#include "Systems/CameraController.h"

using namespace Realisim;
    using namespace Reactor;

//---------------------------------------------------------------------------------------------------------------------
Engine::Engine() {
    createCoreSystems();
}

//---------------------------------------------------------------------------------------------------------------------
Engine::~Engine() {
    terminate();
}

//---------------------------------------------------------------------------------------------------------------------
void Engine::addUserSystem(int iSystemId, ISystem* ipSystem)
{
    assert(ipSystem != nullptr);
    if (ipSystem == nullptr) return;

    //make sure it is not already present;
    if (mHub.mUserDefinedSystems.find(iSystemId) != mHub.mUserDefinedSystems.end()) {
        LOG_TRACE_ERROR(Core::Logger::llNormal, "A system with system id [%d] already exists.", iSystemId);
    }
    else
    {
        mSystems.push_back(ipSystem);
        mHub.mUserDefinedSystems[iSystemId] = ipSystem;
    }
        
}


//---------------------------------------------------------------------------------------------------------------------
void Engine::createCoreSystems() {

    // create and assign systems to the hub
    // renderer
    Renderer *pRenderer = new Renderer(&mBroker, &mHub);

    mSystems.push_back(pRenderer);

    // assign to hub
    mHub.mpRenderer = pRenderer;
}

//---------------------------------------------------------------------------------------------------------------------
bool Engine::init()
{
    bool r = true;

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
    Broker& b = getBrokerRef();
    b.setStartOfFrameTime();

    for (auto s : mSystems)
        s->preUpdate();

    for (auto s : mSystems)
        s->update();

    for (auto s : mSystems)
        s->postUpdate();

    // important
    // after all systems have been updated, we swapbuffers, this will sync with the screen refresh rate
    //
    Reactor::Renderer& r = getHubRef().getRendererRef();
    r.swapBuffers();
}

////---------------------------------------------------------------------------------------------------------------------
//void Engine::start()
//{}
//
////---------------------------------------------------------------------------------------------------------------------
//void Engine::stop()
//{}

//---------------------------------------------------------------------------------------------------------------------
// This function will remove termininate and remove the system from the active list.
// It returns the system to the caller
//
ISystem* Engine::removeUserSystem(int iSystemId)
{
    ISystem* pR = nullptr;

    // remove from user system
    if (mHub.mUserDefinedSystems.find(iSystemId) != mHub.mUserDefinedSystems.end()) {
        pR = mHub.mUserDefinedSystems[iSystemId];
        mHub.mUserDefinedSystems.erase(iSystemId);
    }

    // remove from system list
    auto itSystem = find(mSystems.begin(), mSystems.end(), pR);
    assert(itSystem != mSystems.end());

    if (itSystem == mSystems.end()) {
        LOG_TRACE_ERROR(Core::Logger::llNormal, "System id [%id] was not found in system list...", iSystemId);
    }
    else
    {
        mSystems.erase(itSystem);
    }

    return pR;
}

//---------------------------------------------------------------------------------------------------------------------
void Engine::setNativeWindowsGlContext(HDC iHDC, HGLRC iHGLRC)
{
    mHub.getRendererRef().setNativeWindowsGlContext(iHDC, iHGLRC);
}

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