
#include <cassert>
#include "Core/Logger.h"
#include "Core/Path.h"
#include "DataStructures/Player.h"
#include "Geometry/GeometricGrid.h"
#include "Reactor/DataStructures/Scene/ModelNode.h"
#include "Reactor/Systems/CameraController.h"
#include "Reactor/Systems/Renderer/Renderer.h"
#include "Systems/GameSystem.h"
#include "Systems/PhysicsSystem.h"
#include "Systems/SystemIds.h"
#include "Systems/TerrainEditionSystem.h"
#include "3d/Material.h"

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
    cam.set(Vector3(0, 0, 50), Vector3(0.0), Vector3(0.0, 1.0, 0.0));

    using namespace Geometry;
    float radius = 0.5;
    GeometricGrid geoGrid; geoGrid.set(Vector3(radius), 32, 64);

    ImageNode* playerImageNode = new ImageNode();
    playerImageNode->setName("playerImageNode");
    playerImageNode->setFilenamePath(Core::Path::join(b.getAssetPath(), "Textures/ballTexture.png"));

    ImageNode* playerImageGlowNode = new ImageNode();
    playerImageGlowNode->setName("playerImageGlowNode");
    playerImageGlowNode->setFilenamePath(Core::Path::join(b.getAssetPath(), "Textures/ballTexture_glow.png"));

    MaterialNode* playerMatNode = new MaterialNode();
    playerMatNode->setName("playerMatNode");
    playerMatNode->addImageNode(playerImageNode, ThreeD::Material::ilDiffuse);
    playerMatNode->addImageNode(playerImageGlowNode, ThreeD::Material::ilAdditional0);
    
    ModelNode* playerModelNode = new ModelNode();
    playerModelNode->setName("sphere");
    Mesh m = geoGrid.getMesh();
    playerModelNode->addMesh(m);
    playerModelNode->setMaterialNode(playerMatNode);
    playerModelNode->setRegisteredRenderPasses({ rpiOpaque, rpiGlow });

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

    int iCurrentSpline = mTerrain.addSpline();
    mTerrain.addSegment(iCurrentSpline, ls0);
    mTerrain.addSegment(iCurrentSpline, ls1);
    mTerrain.addSegment(iCurrentSpline, ls2);

    ThreeD::SceneNode& root = scene.getRootRef();
    root.addChild(playerModelNode);
    scene.getImageLibraryRef().addChild(playerImageNode);
    scene.getImageLibraryRef().addChild(playerImageGlowNode);
    scene.getMaterialLibraryRef().addChild(playerMatNode);

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
Geometry::Mesh GameSystem::makeMeshFromTerrainSpline(const TerrainSpline& iTs)
{
    using namespace Geometry;
    Mesh m;
    m.setNumberOfVerticesPerFace(4);
    std::vector<Mesh::VertexData>& verticesRef = m.getVerticesRef();

    const uint32_t numPolygons = (uint32_t)iTs.m2dPolygons.size();
    uint32_t vertexIndex = 0;
    for (uint32_t i = 0; i < numPolygons; ++i)
    {
        Mesh::VertexData vd0, vd1, vd2, vd3, vd4, vd5, vd6, vd7;
        const Polygon2d& p2d = iTs.m2dPolygons[i];
        vd0.mVertex = Vector3(p2d.getVertices()[0], 1.0);
        vd1.mVertex = Vector3(p2d.getVertices()[1], 1.0);
        vd2.mVertex = Vector3(p2d.getVertices()[2], 1.0);
        vd3.mVertex = Vector3(p2d.getVertices()[3], 1.0);

        vd4.mVertex = Vector3(p2d.getVertices()[0], -1.0);
        vd5.mVertex = Vector3(p2d.getVertices()[1], -1.0);
        vd6.mVertex = Vector3(p2d.getVertices()[2], -1.0);
        vd7.mVertex = Vector3(p2d.getVertices()[3], -1.0);

        verticesRef.push_back(vd0);
        verticesRef.push_back(vd1);
        verticesRef.push_back(vd2);
        verticesRef.push_back(vd3);

        verticesRef.push_back(vd4);
        verticesRef.push_back(vd5);
        verticesRef.push_back(vd6);
        verticesRef.push_back(vd7);

        m.makeFace({ vertexIndex + 0, vertexIndex + 1, vertexIndex + 2, vertexIndex + 3 }); //  z
        m.makeFace({ vertexIndex + 4, vertexIndex + 7, vertexIndex + 6, vertexIndex + 5 }); // -z
        m.makeFace({ vertexIndex + 0, vertexIndex + 3, vertexIndex + 7, vertexIndex + 4 }); //  y
        m.makeFace({ vertexIndex + 1, vertexIndex + 5, vertexIndex + 6, vertexIndex + 2 }); // -y
        m.makeFace({ vertexIndex + 0, vertexIndex + 4, vertexIndex + 5, vertexIndex + 1 }); //  x
        m.makeFace({ vertexIndex + 2, vertexIndex + 6, vertexIndex + 7, vertexIndex + 3 }); //  -x

        vertexIndex += 8;
    }

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


    const double minThreshold = 0.15;
    const double maxThreshold = 0.85;
    double deltaInXNormalized = 0.0;
    double deltaInYNormalized = 0.0;
    if (playerPosInPixelNormalized.x() >= maxThreshold) {
        deltaInXNormalized = playerPosInPixelNormalized.x() - maxThreshold; }
    if (playerPosInPixelNormalized.x() <= minThreshold) {
        deltaInXNormalized = playerPosInPixelNormalized.x() - minThreshold; }
    if (playerPosInPixelNormalized.y() >= maxThreshold) {
        deltaInYNormalized = playerPosInPixelNormalized.y() - maxThreshold;
    }
    if (playerPosInPixelNormalized.y() <= minThreshold) {
        deltaInYNormalized = playerPosInPixelNormalized.y() - minThreshold;
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

    // update all dirty terrain splines
    const int numSplines = mTerrain.getNumberOfSplines();
    for (int j = 0; j < numSplines; ++j)
    {
        if (mTerrain.isSplineDirty(j))
        {
            Reactor::ModelNode* modelNode = mTerrain.getSplineModelNodePtr(j);
            if (modelNode != nullptr)
                r.removeRenderableAsSoonAsPossible(modelNode);
            else {
                modelNode = new Reactor::ModelNode();
                modelNode->setName("Spline_" + std::to_string(j));
                modelNode->setRegisteredRenderPasses({ rpiOpaque, rpiPicking });
                root.addChild(modelNode);
                mTerrain.setSplineModelNode(j, modelNode);
            }
            modelNode->clear(); // removes all mesh
            Geometry::Mesh mesh = makeMeshFromTerrainSpline(mTerrain.getSpline(j));
            if(mesh.getNumberOfFaces() > 0)
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

