#include "Rendering/Gpu/OpenGlHeaders.h"


#include "Broker.h"
#include <cassert>
#include "Core/FileInfo.h"
#include "Core/Path.h"
#include "Hub.h"
#include "Renderer.h"
#include "Rendering/Camera.h"
#include "Rendering/Viewport.h"

//-- temporary
#include "DataStructures/Scene/ModelNode.h"
#include "DataStructures/Scene/SceneNodeEnum.h"
#include "Geometry/RectangularPrism.h"
#include "Rendering/Gpu/VertexArrayObjectMaker.h"
#include "Systems/Renderer/ModelRenderable.h"

using namespace std;
using namespace Realisim;
    using namespace Core;
    using namespace Math;
    using namespace Reactor;
    using namespace Rendering;
    using namespace ThreeD;

//---------------------------------------------------------------------------------------------------------------------
Renderer::Renderer(Broker* ipBroker, Hub* ipHub) : ISystem(ipBroker, ipHub),
    mpScene(nullptr),
    mpVbo(nullptr)
{}

//---------------------------------------------------------------------------------------------------------------------
Renderer::~Renderer()
{
    clear();
}

//---------------------------------------------------------------------------------------------------------------------
void Renderer::addRenderable(ThreeD::SceneNode* ipNode, IRenderable* ipRenderable)
{
    const uint32_t id = ipNode->getId();
    const auto itSceneNode = mIdToSceneNode.find(id);
    assert(itSceneNode == mIdToSceneNode.end() && "Inserting the same id more than once is problematic as we loose track of some renderables...");

    mIdToSceneNode[id] = ipNode;
    mIdToRenderable[id] = ipRenderable;
}

//---------------------------------------------------------------------------------------------------------------------
void Renderer::clear()
{
    if (mpVbo != nullptr)
    {
        delete mpVbo;
        mpVbo = nullptr;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void Renderer::draw()
{
    const Camera& cam = getBroker().getMainCamera();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Matrix4 modelMatrix(Vector3(0.0, 0.0, 0.0));

    glUseProgram(mModelShader.getProgramId());

    mModelShader.setUniform("uProjectionMatrix", cam.getProjectionMatrix());
    mModelShader.setUniform("uViewMatrix", cam.getViewMatrix());
    mModelShader.setUniform("uApplyLighting", true);
    mModelShader.setUniform("uLightPosition", Math::Vector3(-1, 0.7, 0.3));
    
    ModelRenderable* pModelRenderable = nullptr;
    for (auto itRenderable : mIdToRenderable) {
        pModelRenderable = (ModelRenderable *)itRenderable.second;

        ModelNode* pNode = pModelRenderable->getModelNode();
        mModelShader.setUniform("uModelMatrix", pNode->getWorldTransform());

        pModelRenderable->draw();
    }

    glUseProgram(0);
}

//---------------------------------------------------------------------------------------------------------------------
bool Renderer::init()
{
    bool r = true;

    // place camera
    Camera& cam = getBrokerRef().getMainCameraRef();
    cam.set(Vector3(10, 10, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));

    return r;
}

//---------------------------------------------------------------------------------------------------------------------
bool Renderer::initializeGl()
{
    bool r = true;
    mContext.makeCurrent();

    // init GLEW
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        printf("Glew initialization failed... Context must be invalid.\n");
        r = false;
    }

    // set glClearColor
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_FRAMEBUFFER_SRGB);

    // basic opengl states.
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);

    // enable vsync
    const int swapInterval = 1; // mConfig.isVSyncEnabled() ? 1 : 0;
    wglSwapIntervalEXT(swapInterval);

    //
    loadShaders();

    // init vbo
    Geometry::RectangularPrism prism;
    prism.set(Vector3(-5), Vector3(5));
    mpVbo = makeVao(prism.makeMesh());


    // init scene
    // init scene
    if (mpScene)
    {
        initializeSceneNode(&(mpScene->getRootRef()));
    }

    mContext.doneCurrent();

    return r;
}

//---------------------------------------------------------------------------------------------------------------------
void Renderer::initializeSceneNode(SceneNode* ipNode)
{
    const Broker& b = getBroker();

    ipNode->update(b.getTimeInSecondsOfStartOfFrame());
    for (int i = 0; i < ipNode->getNumberOfChilds(); ++i)
    {
        initializeSceneNode(ipNode->getChild(i));
    }
    makeAndAddRenderable(ipNode);
}

//---------------------------------------------------------------------------------------------------------------------
void Renderer::handleKeyboard()
{
    const Broker& b = getBroker();
    const Interface::Keyboard& k = b.getKeyboard();

    if (k.isKeyPressed(Key::Key_Control) && k.isKeyPressed(Key::Key_R)) {
        loadShaders();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void Renderer::loadShaders()
{
    printf("Loading Shaders...\n");

    mModelShader.clear();

    //--- shaders
    FileInfo appPath(Path::getApplicationFilePath());
    string assetsPath = Path::join(appPath.getAbsolutePath(), "../Assets");

    mModelShader.setName("modelShader");
    mModelShader.addSourceFromFile(stVertex, Path::join(assetsPath, "Shaders/model.vert"));
    mModelShader.addSourceFromFile(stFragment, Path::join(assetsPath, "Shaders/modelWithMaterial.frag"));
    mModelShader.compile();
    mModelShader.link();

    if (mModelShader.hasErrors())
    {
        cout << mModelShader.getAndClearLastErrors();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void Renderer::makeAndAddRenderable(SceneNode* ipNode)
{
    switch ((int)ipNode->getNodeType())
    {
    case (int)SceneNodeEnum::sneModelNode: {
        ModelNode* n = (ModelNode*)ipNode;
        ModelRenderable* r = new ModelRenderable(n);
        r->initializeGpuRessources();

        addRenderable(ipNode, r);
    } break;
    default: break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void Renderer::preUpdate() {
}

//---------------------------------------------------------------------------------------------------------------------
void Renderer::update(){
    mContext.makeCurrent();
    
    // draw
    draw();

    handleKeyboard();
    mContext.doneCurrent();
}

//---------------------------------------------------------------------------------------------------------------------
void Renderer::resizeGl(int iWidth, int iHeight)
{
    mContext.makeCurrent();

    Camera& cam = getBrokerRef().getMainCameraRef();

    Rendering::Viewport v(iWidth, iHeight);

    // reset camera viewport
    cam.setViewport(v);
    glViewport(0, 0, iWidth, iHeight);

    mContext.doneCurrent();
}


//---------------------------------------------------------------------------------------------------------------------
void Renderer::setScene(Scene* ipScene)
{
    // flush all previous renderables

    mpScene = ipScene;
}

//---------------------------------------------------------------------------------------------------------------------
void Renderer::setNativeWindowsGlContext(HDC iHDC, HGLRC iHGLRC)
{
    mDevice.createGlContextFromWindowsHandles(iHDC, iHGLRC);
    mContext = Rendering::Context();
    if (mDevice.getNumberOfContexts() > 0)
    {
        mContext = mDevice.getContext(mDevice.getNumberOfContexts() - 1);
    }

    initializeGl();
    const Camera& cam = getBroker().getMainCamera();
    resizeGl(cam.getViewport().getWidth(), cam.getViewport().getHeight());
}

//---------------------------------------------------------------------------------------------------------------------
void Renderer::swapBuffers() {
    mContext.makeCurrent();

    SwapBuffers(mContext.getDC());
    //glFinish();

    mContext.doneCurrent();
}