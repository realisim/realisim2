
#include <cassert>
#include "Core/Logger.h"
#include "Reactor/Systems/Renderer/Renderer.h"
#include "Math/Vector.h"
#include "Systems/GameSystem.h"
#include "Systems/TerrainEditionSystem.h"
#include "Systems/PhysicsSystem.h"
#include "Systems/SystemIds.h"

using namespace Realisim;
using namespace Interface;
using namespace Math;
using namespace Reactor;
using namespace Synthroll;

//---------------------------------------------------------------------------------------------------------------------
TerrainEditionSystem::TerrainEditionSystem(Reactor::Broker* ipBroker, Reactor::Hub* ipHub) : ISystem(ipBroker, ipHub),
mState(sIdle),
mCurrentSegmentIndex(-1)
{}

//---------------------------------------------------------------------------------------------------------------------
TerrainEditionSystem::~TerrainEditionSystem()
{
}

//---------------------------------------------------------------------------------------------------------------------
void TerrainEditionSystem::addNewSegment() {
    

    GameSystem *gs = (GameSystem*)getHubRef().getUserDefinedSystemPtr(GAME_SYSTEM_ID);
    Terrain& terrain = gs->getTerrainRef();
    const Mouse &m = getBroker().getMouse();

    Geometry::LineSegment l;
    l.mP0 = mousePosToWorld(m.getPosition().x(), m.getPosition().y());
    l.mP1 = l.mP0;

    mCurrentSegmentIndex = terrain.addSegment(l);
}

//---------------------------------------------------------------------------------------------------------------------
void TerrainEditionSystem::addPointToSegments() {
    GameSystem* gs = (GameSystem*)getHubRef().getUserDefinedSystemPtr(GAME_SYSTEM_ID);
    Terrain& terrain = gs->getTerrainRef();
    const Mouse& m = getBroker().getMouse();

    Geometry::LineSegment &l = terrain.getSegmentRef(mCurrentSegmentIndex);
    l.mP1 = mousePosToWorld(m.getPosition().x(), m.getPosition().y());

    Geometry::LineSegment l2;
    l2.mP0 = l.mP1;
    l2.mP1 = l2.mP0;

    mCurrentSegmentIndex = terrain.addSegment(l2);
}

//---------------------------------------------------------------------------------------------------------------------
void TerrainEditionSystem::endSegment() {
    GameSystem* gs = (GameSystem*)getHubRef().getUserDefinedSystemPtr(GAME_SYSTEM_ID);
    Terrain& terrain = gs->getTerrainRef();
    const Mouse& m = getBroker().getMouse();

    Geometry::LineSegment& l = terrain.getSegmentRef(mCurrentSegmentIndex);
    l.mP1 = mousePosToWorld(m.getPosition().x(), m.getPosition().y());

    terrain.setSegmentAsDirty(mCurrentSegmentIndex, true);
}

//---------------------------------------------------------------------------------------------------------------------
void TerrainEditionSystem::handleKeyboard() {
    Keyboard& k = getBrokerRef().getKeyboardRef();

    GameSystem* gs = (GameSystem*)getHubRef().getUserDefinedSystemPtr(GAME_SYSTEM_ID);
    Terrain& terrain = gs->getTerrainRef();

    PhysicsSystem* ps = (PhysicsSystem*)getHubRef().getUserDefinedSystemPtr(PHYSICS_SYSTEM_ID);

    if (k.isKeyPressed(Key_Space))
    {
        switch (ps->getState())
        {
        case PhysicsSystem::sPaused: ps->resume(); break;
        case PhysicsSystem::sRunning: ps->pause(); break;
        default: assert(false);  break;
        }

        k.setKeyReleased(Key_Space);
    }

    if (k.isKeyPressed(Key_R))
    {
        ps->reset();
        k.setKeyReleased(Key_R);
    }

    if (k.isKeyPressed(Key_Backspace))
    {
        const int lastSegmentId = terrain.getNumberOfSegments() - 1;
        if(lastSegmentId >= 0)
            removeSegment(lastSegmentId);
        k.setKeyReleased(Key_Backspace);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TerrainEditionSystem::handleMouse() {
    Mouse& m = getBrokerRef().getMouseRef();

    if (m.isButtonPressed(Mouse::bLeft))
    {
        setState(sAddPoints);
        m.setButtonReleased(m.getPosition().x(), m.getPosition().y(), Mouse::bLeft);
    }
    else if (m.isButtonPressed(Mouse::bRight)) {
        setState(sIdle);
        m.setButtonReleased(m.getPosition().x(), m.getPosition().y(), Mouse::bRight);
    }
    else {
        if (mState == sAddPoints)
        {
            // simple mouse move...
            // move the last point of the segment
            GameSystem* gs = (GameSystem*)getHubRef().getUserDefinedSystemPtr(GAME_SYSTEM_ID);
            Terrain& terrain = gs->getTerrainRef();

            Geometry::LineSegment& l = terrain.getSegmentRef(mCurrentSegmentIndex);
            l.mP1 = mousePosToWorld(m.getPosition().x(), m.getPosition().y());

            terrain.setSegmentAsDirty(mCurrentSegmentIndex, true);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool TerrainEditionSystem::init() {
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
Vector3 TerrainEditionSystem::mousePosToWorld(int iX, int iY) {
    const Rendering::Camera& cam = getBroker().getMainCamera();
    Vector3 referencePoint = cam.getPosition();
    referencePoint.setZ(0.0);

    int y = cam.getViewport().getHeight() - iY; //qt and opengl have inverted y.

    const Vector3 p = cam.pixelToWorld(Vector2((double)iX, (double)y), referencePoint);
    return p;
}

//---------------------------------------------------------------------------------------------------------------------
void TerrainEditionSystem::removeSegment(int iSegmentId){
    GameSystem* gs = (GameSystem*)getHubRef().getUserDefinedSystemPtr(GAME_SYSTEM_ID);
    PhysicsSystem* ps = (PhysicsSystem*)getHubRef().getUserDefinedSystemPtr(PHYSICS_SYSTEM_ID);
    Terrain& terrain = gs->getTerrainRef();

    const ModelNode* modelNode = terrain.getModelNode(iSegmentId);

    Broker& b = getBrokerRef();
    Scene& scene = b.getSceneRef();
    ThreeD::SceneNode& root = scene.getRootRef();

    // remove from scene and renderer
#pragma message("IL FAUT DELETE LE nodeToRemove DANS LES PROCHAINS FRAMES....")
    // peut etre demander au renderer d<enlever le renderable et deleter le modelNode associé...
    ThreeD::SceneNode* nodeToRemove = root.removeNode(modelNode->getId());
    if (nodeToRemove != nullptr) {
        Renderer& r = getHubRef().getRendererRef();
        r.removeRenderableAsSoonAsPossible(modelNode->getId());
    }

    // remove from physics
    ps->removeSegment(iSegmentId);

    // remove from terrain
    terrain.removeSegment(iSegmentId);
}

//---------------------------------------------------------------------------------------------------------------------
void TerrainEditionSystem::setState(State iS)
{
    switch (mState)
    {
    case TerrainEditionSystem::sIdle:
        if (iS == sAddPoints) {
            addNewSegment();
        }
        break;
    case TerrainEditionSystem::sAddPoints:
        switch (iS)
        {
        case TerrainEditionSystem::sIdle: endSegment(); break;
        case TerrainEditionSystem::sAddPoints: addPointToSegments(); break;
        default: break;
        }
        break;
    default: break;
    }

    mState = iS;
}

//---------------------------------------------------------------------------------------------------------------------
void TerrainEditionSystem::update()
{
    GameSystem* gs = (GameSystem*)getHubRef().getUserDefinedSystemPtr(GAME_SYSTEM_ID);
    Terrain& terrain = gs->getTerrainRef();

    // clear all segment that were dirty from previous frame.
    const int numSegments = terrain.getNumberOfSegments();
    for (int i = 0; i < numSegments; ++i)
    {
        terrain.setSegmentAsDirty(i, false);
    }

    handleKeyboard();
    handleMouse();
}
