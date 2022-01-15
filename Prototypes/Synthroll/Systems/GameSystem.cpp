
#include <cassert>
#include "Core/Logger.h"
#include "DataStructures/Player.h"
#include "Geometry/GeometricGrid.h"
#include "Reactor/DataStructures/Scene/ModelNode.h"
#include "Reactor/Systems/CameraController.h"
#include "Reactor/Systems/Renderer/Renderer.h"
#include "Systems/GameSystem.h"
#include "Systems/PhysicsSystem.h"
#include "Systems/SystemIds.h"
#include "Systems/TerrainEditionSystem.h"

using namespace Realisim;
using namespace Math;
using namespace Reactor;
using namespace Synthroll;

//---------------------------------------------------------------------------------------------------------------------
GameSystem::GameSystem(Reactor::Broker* ipBroker, Reactor::Hub* ipHub) : ISystem(ipBroker, ipHub),
mMode(mNone),
mpPlayer0(nullptr)
{
    
}

//---------------------------------------------------------------------------------------------------------------------
GameSystem::~GameSystem()
{
    if (mpPlayer0)
        delete mpPlayer0;
}

//---------------------------------------------------------------------------------------------------------------------
bool GameSystem::init() {
    // create the player and add to scene
    Broker& b = getBrokerRef();
    //b.makeBasicScene();

    Scene& scene = b.getSceneRef();
    scene.setAmbientFactor( Vector3(0.3) );

    Rendering::Camera& cam = b.getMainCameraRef();
    cam.set(Vector3(0, 0, 30), Vector3(0.0), Vector3(0.0, 1.0, 0.0));

    using namespace Geometry;
    float radius = 0.5;
    GeometricGrid geoGrid; geoGrid.set(Vector3(radius), 32, 64);

    ModelNode *playerModelNode = new ModelNode();
    playerModelNode->setName("sphere");
    Mesh m = geoGrid.getMesh();
    playerModelNode->addMesh(m);

    mpPlayer0 = new Player();
    mpPlayer0->setRadius(radius);
    mpPlayer0->setModelNode(playerModelNode);

    Renderer& r = getHubRef().getRendererRef();
    r.addAndMakeRenderableAsSoonAsPossible(playerModelNode);


    // load/create a terrain
    // create 3 line segments
    LineSegment ls0(Vector3(-10, 5, 0), Vector3(-5, 0, 0));
    LineSegment ls1(Vector3(-5, 0, 0), Vector3(5, 0.5, 0));
    LineSegment ls2(Vector3(5, 0.5, 0), Vector3(10, 5, 0));

    mTerrain.addSegment(ls0);
    mTerrain.addSegment(ls1);
    mTerrain.addSegment(ls2);

    ThreeD::SceneNode& root = scene.getRootRef();
    root.addChild(playerModelNode);

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
Geometry::Mesh GameSystem::makeMeshFromTerrainSegment(const Geometry::LineSegment& iLineSegment)
{
    using namespace Geometry;
    Mesh m;
    m.setNumberOfVerticesPerFace(4);
    std::vector<Mesh::VertexData>& verticesRef = m.getVerticesRef();
    Mesh::VertexData vd0, vd1, vd2, vd3;

    Vector3 perpendicularDir = (iLineSegment.mP1 - iLineSegment.mP0).cross(Vector3(0.0, 0.0, 1.0));
    perpendicularDir.normalize();

    vd0.mVertex = iLineSegment.mP0;
    vd1.mVertex = iLineSegment.mP0 + (perpendicularDir * 1);
    vd2.mVertex = iLineSegment.mP1 + (perpendicularDir * 1);
    vd3.mVertex = iLineSegment.mP1;

    verticesRef.push_back(vd0);
    verticesRef.push_back(vd1);
    verticesRef.push_back(vd2);
    verticesRef.push_back(vd3);
    m.makeFace( { 0, 1, 2, 3 } );

    m.triangulate();
    m.generateFlatNormals();

    return m;
}

//---------------------------------------------------------------------------------------------------------------------
bool GameSystem::postInit() {
    setMode(mTerrainEdition);
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
void GameSystem::setMode(Mode iV)
{
    //disable all handlers
    Hub& h = getHubRef();

    CameraController* pCameraControler = (CameraController*)h.getUserDefinedSystemPtr(Hub::usiCameraController);
    pCameraControler->setAsActive(false);

    PhysicsSystem* pPhysics = (PhysicsSystem*)h.getUserDefinedSystemPtr(PHYSICS_SYSTEM_ID);
    pPhysics->setAsActive(false);

    TerrainEditionSystem* pTerrainEditionSystem = (TerrainEditionSystem*)h.getUserDefinedSystemPtr(TERRAIN_EDITION_SYSTEM_ID);
    pTerrainEditionSystem->setAsActive(false);

    switch (iV)
    {
    case GameSystem::mNone: break;
    case GameSystem::mMainMenu:
        //enable main menu handlers
        //flush all renderables
        //diaply main menu screen
        break;
    case GameSystem::mPlaying:
        //enable play handlers
        //flush all renderables
        //display game
        break;
    case GameSystem::mTerrainEdition:
        //enable terrain Edition handlers
        pTerrainEditionSystem->setAsActive(true);

        pPhysics->setTerrain(&mTerrain);
        pPhysics->setAsActive(true);
        //flush all renderables
        //diaply game
        break;
    default: assert(false); break;
    }

    mMode = iV;
}

//---------------------------------------------------------------------------------------------------------------------
void GameSystem::update()
{
    Broker& b = getBrokerRef();
    Scene& scene = b.getSceneRef();
    ThreeD::SceneNode& root = scene.getRootRef();

    if (mTerrain.isDirty())
    { updateTerrain(); }

    updateCamera();

    // update the scene graph
    updateSceneGraph(&root);
}

//---------------------------------------------------------------------------------------------------------------------
void GameSystem::updateCamera() {
    Broker& b = getBrokerRef();
    Rendering::Camera& cam = b.getMainCameraRef();
    
    const ModelNode& playerNode = mpPlayer0->getModelNode();
    Vector3 playerPos = playerNode.getWorldTransform().getTranslationAsVector();

    const Rendering::Viewport& vp = cam.getViewport();
    Vector2 playerPosInPixel = cam.worldToPixel(playerPos);
    Vector2 playerPosInPixelNormalized(playerPosInPixel.x() / (double)vp.getWidth(), playerPosInPixel.y() / (double)vp.getHeight());

    double deltaInXNormalized = 0.0;
    double deltaInYNormalized = 0.0;
    if (playerPosInPixelNormalized.x() >= 0.7) {
        deltaInXNormalized = playerPosInPixelNormalized.x() - 0.7; }
    if (playerPosInPixelNormalized.x() <= 0.3) {
        deltaInXNormalized = playerPosInPixelNormalized.x() - 0.3; }
    if (playerPosInPixelNormalized.y() >= 0.7) {
        deltaInYNormalized = playerPosInPixelNormalized.y() - 0.7;
    }
    if (playerPosInPixelNormalized.y() <= 0.3) {
        deltaInYNormalized = playerPosInPixelNormalized.y() - 0.3;
    }
    
    Vector2 deltaInPixel(deltaInXNormalized * vp.getWidth(), deltaInYNormalized * vp.getHeight());
    Vector3 deltaInWorld = cam.pixelDeltaToWorld(playerPosInPixel, deltaInPixel, cam.getFocal());

    cam.translate(deltaInWorld);
}

//---------------------------------------------------------------------------------------------------------------------
void GameSystem::updateTerrain()
{
    Broker& b = getBrokerRef();
    Scene& scene = b.getSceneRef();
    ThreeD::SceneNode& root = scene.getRootRef();
    Renderer& r = getHubRef().getRendererRef();

    // update all dirty terrain segments
    const int numSegments = mTerrain.getNumberOfSegments();
    for (int i = 0; i < numSegments; ++i)
    {
        const Geometry::LineSegment& l = mTerrain.getSegment(i);
        if (mTerrain.isSegmentDirty(i))
        {
            Reactor::ModelNode* modelNode = mTerrain.getModelNodePtr(i);
            if(modelNode != nullptr)
                r.removeRenderableAsSoonAsPossible(modelNode);
            else {
                modelNode = new Reactor::ModelNode();
                modelNode->setName("LineSegment_" + std::to_string(i));
                root.addChild(modelNode);
                mTerrain.setModelNode(i, modelNode);
            }

            modelNode->clear(); // removes all mesh
            Geometry::Mesh mesh = makeMeshFromTerrainSegment(l);
            modelNode->addMesh(mesh);

            r.addAndMakeRenderableAsSoonAsPossible(modelNode);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void GameSystem::updateSceneGraph(ThreeD::SceneNode* ipNode)
{
    const Broker& b = getBroker();
    ipNode->update(b.getTimeInSecondsOfStartOfFrame());
    for (int i = 0; i < ipNode->getNumberOfChilds(); ++i)
    {
        updateSceneGraph(ipNode->getChild(i));
    }
}

