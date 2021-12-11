#include "Rendering/Gpu/OpenGlHeaders.h"


#include "Broker.h"
#include <cassert>
#include "Core/FileInfo.h"
#include "Core/Logger.h"
#include "Core/Path.h"
#include "Hub.h"
#include "Renderer.h"
#include "Rendering/Camera.h"
#include "Rendering/Viewport.h"
#include "Systems/Renderer/IRenderable.h"
#include "Systems/Renderer/ModelRenderable.h"
#include "Systems/Renderer/TextureRenderable.h"
#include "Systems/Renderer/RenderPasses/OpaquePass.h"
#include "Systems/Renderer/RenderPasses/GlowPass.h"
#include "Systems/Renderer/RenderPasses/ScreenBlitPass.h"


//-- temporary
#include "DataStructures/Scene/ModelNode.h"
#include "DataStructures/Scene/SceneNodeEnum.h"


using namespace std;
using namespace Realisim;
    using namespace Core;
    using namespace Math;
    using namespace Reactor;
    using namespace Rendering;
    using namespace ThreeD;

//---------------------------------------------------------------------------------------------------------------------
Renderer::Renderer(Broker* ipBroker, Hub* ipHub) : ISystem(ipBroker, ipHub),
    mpScene(nullptr)
{
    OpaquePass* pOpaquePass = new OpaquePass();
    pOpaquePass->setName("OpaquePass");
    addRenderPass(rpiOpaque, pOpaquePass);

    GlowPass* pGlowPass = new GlowPass();
    pGlowPass->setName("GlowPass");
    addRenderPass(rpiGlow, pGlowPass);

    ScreenBlitPass* pScreenBlit = new ScreenBlitPass();
    pScreenBlit->setName("ScreenBlitPass");
    addRenderPass(rpiScreenBlit, pScreenBlit);
}

//---------------------------------------------------------------------------------------------------------------------
Renderer::~Renderer()
{
    clear();
}

//---------------------------------------------------------------------------------------------------------------------
void Renderer::addAndMakeRenderable(SceneNode* ipNode)
{
    switch ((int)ipNode->getNodeType())
    {
    case (int)SceneNodeEnum::sneModelNode: {

        //init model
        ModelNode* n = (ModelNode*)ipNode;
        ModelRenderable* mr = new ModelRenderable(n);
        mr->initializeGpuRessources();

        //init or find textures
        MaterialNode* matNode = n->getMaterialNode();
        for (int i = 0; i < ThreeD::Material::ilNumberOfLayers && matNode; ++i) {
            TextureRenderable* tr = nullptr;
            ImageNode* imageNode = matNode->getImageNode((ThreeD::Material::ImageLayer)i);
            if (imageNode != nullptr)
            {
                Image& im = imageNode->getImageRef();
                if (!im.hasImageData())
                {
                    im.load();
                    tr = new TextureRenderable(imageNode);
                    tr->initializeGpuRessources();
                    addTextureRenderable(imageNode, tr);
                }
                else {
                    // find the associated TextureRenderable
                    auto itTex = mIdToTexture.find(imageNode->getId());
                    assert(itTex != mIdToTexture.end());
                    tr = itTex->second;
                }
                mr->setTexture(&tr->getTexture(), (ThreeD::Material::ImageLayer)i);
            }
        }

        addDrawable(ipNode, mr);
        for (auto passId : n->getRegisteredRenderPasses()) {
            mPassIdToDrawables[passId].push_back(mr);
        }

    } break;
    default: break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void Renderer::addDrawable(ThreeD::SceneNode* ipNode, IRenderable* ipRenderable)
{
    const uint32_t id = ipNode->getId();
    const auto itSceneNode = mIdToSceneNode.find(id);
    assert(itSceneNode == mIdToSceneNode.end());
    if (itSceneNode != mIdToSceneNode.end())
        LOG_TRACE_ERROR(Logger::llNormal, "Inserting the same id more than once is problematic as we loose track of some renderables...");

    mIdToSceneNode[id] = ipNode;
    mIdToRenderable[id] = ipRenderable;
    mIdToDrawable[id] = ipRenderable;
}

// -------------------------------------------------------------------------------------------------------------------- -
void Renderer::addTextureRenderable(ImageNode* ipNode, TextureRenderable* ipTexture)
{
    const uint32_t id = ipNode->getId();
    const auto itSceneNode = mIdToSceneNode.find(id);
    assert(itSceneNode == mIdToSceneNode.end());
    if (itSceneNode != mIdToSceneNode.end())
        LOG_TRACE_ERROR(Logger::llNormal, "Inserting the same id more than once is problematic as we loose track of some renderables...");

    mIdToSceneNode[id] = ipNode;
    mIdToRenderable[id] = ipTexture;
    mIdToTexture[id] = ipTexture;
}

//---------------------------------------------------------------------------------------------------------------------
void Renderer::addRenderPass(int iRenderPassIndex, IRenderPass* ipPass)
{
    bool passAlreadyPresent = mRenderPassIdToRenderPassPtr.find(iRenderPassIndex) != mRenderPassIdToRenderPassPtr.end();
    if (passAlreadyPresent)
    {
        LOG_TRACE(Logger::llNormal, "Trying to add render pass [name: %s, id: %d] but id %d is already in use. The pass will be deleted.",
            ipPass->getName().c_str(), iRenderPassIndex, iRenderPassIndex);
        delete ipPass;
    }
    else
    {
        LOG_TRACE(Logger::llNormal, "Adding render pass [name: %s, id: %d]", ipPass->getName().c_str(), iRenderPassIndex);
        mRenderPassIdToRenderPassPtr[iRenderPassIndex] = ipPass;
        mRenderPasses.push_back(ipPass);
    }

}

//---------------------------------------------------------------------------------------------------------------------
void Renderer::addRenderPass(RenderPassId iId, IRenderPass* ipPass)
{
    addRenderPass((int)iId, ipPass);
}

//---------------------------------------------------------------------------------------------------------------------
void Renderer::clear()
{
    // clear all renderables
    mIdToSceneNode.clear();

    for (auto itRenderable : mIdToRenderable) {
        delete itRenderable.second;
    }
    mIdToRenderable.clear();

    mIdToDrawable.clear();
    mIdToTexture.clear();

    // delete all render pass
    for (auto itRenderPass : mRenderPassIdToRenderPassPtr)
    {
        delete itRenderPass.second;
    }
    mRenderPassIdToRenderPassPtr.clear();
    mRenderPasses.clear();
}

//---------------------------------------------------------------------------------------------------------------------
void Renderer::draw()
{
    const Camera& cam = getBroker().getMainCamera();

    // draw all render pass
    for (auto pPass : mRenderPasses) {
        pPass->applyGlState();
        pPass->render(cam, mPassIdToDrawables[pPass->getId()]);
        pPass->revertGlState();
    }
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
        LOG_TRACE(Core::Logger::llNormal, "Glew initialization failed... Context must be invalid.");
        r = false;
    }

    // set glClearColor
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_FRAMEBUFFER_SRGB);

    // basic opengl states.
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);

    // enable vsync
    const int swapInterval = 1; // mConfig.isVSyncEnabled() ? 1 : 0;
    wglSwapIntervalEXT(swapInterval);


    initializePasses();

    // init scene
    if (mpScene)
    {
        initializeSceneNode(&(mpScene->getRootRef()));
    }

    mContext.doneCurrent();

    return r;
}

//---------------------------------------------------------------------------------------------------------------------
void Renderer::initializePasses() {
    LOG_TRACE(Logger::llNormal, "Initializing render passes...");

    // init all passes
    for (auto pPass : mRenderPasses) {
        pPass->initializeFbo();
        pPass->loadShader(getBroker().getAssetPath());
    }

    // share fbos between pass and terminate initialization
    for (auto pPass : mRenderPasses) {
        pPass->defineInputOutputs(); }

    for (auto pPass : mRenderPasses) {
        pPass->sharePasses(mRenderPassIdToRenderPassPtr); }

    for (auto pPass : mRenderPasses) {
        pPass->connectInputOutputs(); }
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
    addAndMakeRenderable(ipNode);
}

//---------------------------------------------------------------------------------------------------------------------
void Renderer::handleKeyboard()
{
    const Broker& b = getBroker();
    const Interface::Keyboard& k = b.getKeyboard();

    if (k.isKeyPressed(Key::Key_Control) && k.isKeyPressed(Key::Key_R)) {
        reloadShaders();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void Renderer::reloadShaders()
{
    LOG_TRACE(Logger::llNormal, "Reloading render pass shaders...");

    for (auto pPass : mRenderPasses) {
        pPass->loadShader(getBroker().getAssetPath());
    }

    LOG_TRACE(Logger::llNormal, "done loading render pass shaders.");
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

    // resize all renderpass
    for (auto pPass : mRenderPasses) {
        pPass->resize(iWidth, iHeight);
    }

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