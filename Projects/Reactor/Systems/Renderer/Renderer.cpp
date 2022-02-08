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
#include "Systems/Renderer/RenderPasses/CompositingPass.h"
#include "Systems/Renderer/RenderPasses/ContourHighlightPass.h"
#include "Systems/Renderer/RenderPasses/OpaquePass.h"
#include "Systems/Renderer/RenderPasses/GlowPass.h"
#include "Systems/Renderer/RenderPasses/PickingPass.h"
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

    PickingPass* pPickingPass = new PickingPass();
    pPickingPass->setName("PicksingPass");
    addRenderPass(rpiPicking, pPickingPass);

    GlowPass* pGlowPass = new GlowPass();
    pGlowPass->setName("GlowPass");
    addRenderPass(rpiGlow, pGlowPass);

    ContourHighlightPass* pContourHighlightPass = new ContourHighlightPass();
    pContourHighlightPass->setName("ContourHighlightPass");
    addRenderPass(rpiContourHighlight, pContourHighlightPass);

    CompositingPass* pCompositingPass = new CompositingPass();
    pCompositingPass->setName("CompositingPass");
    addRenderPass(rpiCompositing, pCompositingPass);

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
void Renderer::addAndMakeRenderableAsSoonAsPossible(SceneNode* ipNode)
{
    mRenderablesToAddAsSoonAsPossible.push_back(ipNode);
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
void Renderer::addToRenderPass(uint32_t iSceneNodeId, int iRenderPassId)
{
    auto itDrawable = mIdToDrawable.find(iSceneNodeId);
    auto itSceneNode = mIdToSceneNode.find(iSceneNodeId);

    if (itDrawable != mIdToDrawable.end())
    {
        vector<IRenderable*>& passDrawables = mPassIdToDrawables[iRenderPassId];

        auto itDrawableInRenderPass = std::find(passDrawables.begin(), passDrawables.end(), itDrawable->second);
        if (itDrawableInRenderPass == passDrawables.end())
            passDrawables.push_back(itDrawable->second);
    }

    if (itSceneNode != mIdToSceneNode.end())
    {
        SceneNode* sceneNode = itSceneNode->second;
        if ((int)sceneNode->getNodeType() == (int)SceneNodeEnum::sneModelNode) {
            ModelNode* mn = (ModelNode*)sceneNode;
            mn->addRegisteredRenderPass(iRenderPassId);
        }

    }
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
void Renderer::connectBuiltInPasses()
{
    OpaquePass* pOpaquePass = (OpaquePass*)mRenderPassIdToRenderPassPtr[rpiOpaque];
    //PickingPass* pPickingPass = (PickingPass*)mRenderPassIdToRenderPassPtr[rpiPicking];
    GlowPass* pGlowPass = (GlowPass*)mRenderPassIdToRenderPassPtr[rpiGlow];
    ContourHighlightPass* pContourHighlightPass = (ContourHighlightPass*)mRenderPassIdToRenderPassPtr[rpiContourHighlight];
    CompositingPass* pCompositingPass = (CompositingPass*)mRenderPassIdToRenderPassPtr[rpiCompositing];
    ScreenBlitPass* pScreenBlitPass = (ScreenBlitPass*)mRenderPassIdToRenderPassPtr[rpiScreenBlit];

    pCompositingPass->connect(pOpaquePass, "colorOut", "samplerBase");
    pCompositingPass->connect(pGlowPass, "glow1Out", "samplerToAdd0");
    pCompositingPass->connect(pContourHighlightPass, "countourHighlightOut", "samplerToAdd1");

    // connect final screenBlit input-output
    //
    pScreenBlitPass->connect(pCompositingPass, "composedOut", "screenBlitInput");
    //pScreenBlitPass->connect(pPickingPass, "PickingPassTextureOut", "screenBlitInput");
    //pScreenBlitPass->connect(pGlowPass, "opaqueGlowTexturedOut", "screenBlitInput");
    //pScreenBlitPass->connect(pContourHighlightPass, "drawSelectedModelsOut", "screenBlitInput");
    //pScreenBlitPass->connect(pContourHighlightPass, "countourHighlightOut", "screenBlitInput");
    
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
const SceneNode* Renderer::findNode(uint32_t iNodeId) const
{
    SceneNode* r = nullptr;
    auto it = mIdToSceneNode.find(iNodeId);
    if (it != mIdToSceneNode.end())
        r = it->second;
    return r;
}

//---------------------------------------------------------------------------------------------------------------------
SceneNode* Renderer::findNodePtr(uint32_t iNodeId)
{
    return const_cast<SceneNode*>(const_cast<const Renderer*>(this)->findNode(iNodeId));
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
    //glEnable(GL_FRAMEBUFFER_SRGB);

    // basic opengl states.
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);

    // enable vsync
    const int swapInterval = 1; // mConfig.isVSyncEnabled() ? 1 : 0;
    wglSwapIntervalEXT(swapInterval);


    initializePasses();
    connectBuiltInPasses();

    // init scene
    if (mpScene)
    {
        initializeSceneNode(&(mpScene->getRootRef()));
    }

    mContext.doneCurrent();

    mContext.setAsInitialized(r);
    return r;
}

//---------------------------------------------------------------------------------------------------------------------
void Renderer::initializePasses() {
    LOG_TRACE(Logger::llNormal, "Initializing render passes...");
    const Broker& b = getBroker();

    // init all passes
    for (auto pPass : mRenderPasses) {
        pPass->setScene(&b.getScene());
        pPass->initializeFbo();
        pPass->loadShader(b.getAssetPath());
    }

    // share fbos between pass and terminate initialization    
    for (auto pPass : mRenderPasses) {
        pPass->sharePasses(mRenderPassIdToRenderPassPtr); }

    for (auto pPass : mRenderPasses) {
        pPass->defineInputOutputs();
    }
}

//---------------------------------------------------------------------------------------------------------------------
//Should be removed from renderer
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
uint32_t Renderer::pick(int iPixelX, int iPixelY)
{
    uint32_t pickedId = SceneNode::mInvalidId;

    if (!mContext.isInitialized())
        return pickedId;

    mContext.makeCurrent();
    
    PickingPass* pPickingPass = (PickingPass*)mRenderPassIdToRenderPassPtr[rpiPicking];
    if(pPickingPass)
    {
        pickedId = pPickingPass->pick(iPixelX, iPixelY);
    }

    mContext.doneCurrent();

    return pickedId;
}

//---------------------------------------------------------------------------------------------------------------------
void Renderer::preUpdate() {
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
void Renderer::removeFromAllPasses(IRenderable* ipToRemove)
{
    for (auto& itPassIdToDrawables : mPassIdToDrawables) {
        std::vector<IRenderable*>& drawables = itPassIdToDrawables.second;
        auto foundIt = std::find(drawables.begin(), drawables.end(), ipToRemove);
        if (foundIt != drawables.end())
        {
            drawables.erase(foundIt);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void Renderer::removeFromRenderPass(uint32_t iSceneNodeId, int iRenderPassId)
{
    auto itDrawable = mIdToDrawable.find(iSceneNodeId);
    auto itSceneNode = mIdToSceneNode.find(iSceneNodeId);

    if (itDrawable != mIdToDrawable.end())
    {
        vector<IRenderable*>& passDrawables = mPassIdToDrawables[iRenderPassId];

        auto itDrawableInRenderPass = std::find(passDrawables.begin(), passDrawables.end(), itDrawable->second);
        if (itDrawableInRenderPass != passDrawables.end())
            passDrawables.erase(itDrawableInRenderPass);
    }

    if (itSceneNode != mIdToSceneNode.end())
    {
        SceneNode* sceneNode = itSceneNode->second;
        if ((int)sceneNode->getNodeType() == (int)SceneNodeEnum::sneModelNode) {
            ModelNode* mn = (ModelNode*)sceneNode;
            mn->removeRegisteredRenderPass(iRenderPassId);
        }

    }
}

//---------------------------------------------------------------------------------------------------------------------
void Renderer::removeRenderableAsSoonAsPossible(uint32_t iNodeId)
{
    mRenderablesToRemoveAsSoonAsPossible.push_back(iNodeId);
}

//---------------------------------------------------------------------------------------------------------------------
void Renderer::removeRenderableAsSoonAsPossible(const ThreeD::SceneNode* ipNode)
{
    removeRenderableAsSoonAsPossible(ipNode->getId());
}

//---------------------------------------------------------------------------------------------------------------------
void Renderer::removeRenderable(int iNodeId)
{
    auto itSceneNode = mIdToSceneNode.find(iNodeId);
    if (itSceneNode != mIdToSceneNode.end())
        mIdToSceneNode.erase(itSceneNode);

    auto itRenderable = mIdToRenderable.find(iNodeId);
    if (itRenderable != mIdToRenderable.end())
    {
        IRenderable* pRenderableToDelete = itRenderable->second;
        removeFromAllPasses(pRenderableToDelete);
        delete pRenderableToDelete;
        mIdToRenderable.erase(itRenderable);
    }
        
    auto itDrawable = mIdToDrawable.find(iNodeId);
    if (itDrawable != mIdToDrawable.end())
        mIdToDrawable.erase(itDrawable);

    auto itTexture = mIdToTexture.find(iNodeId);
    if (itTexture != mIdToTexture.end())
        mIdToTexture.erase(itTexture);
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
// THIS SHOULD BE REMOVED
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

//---------------------------------------------------------------------------------------------------------------------
void Renderer::update() {
    mContext.makeCurrent();

    for (auto nodeId : mRenderablesToRemoveAsSoonAsPossible) {
        removeRenderable(nodeId); }
    mRenderablesToRemoveAsSoonAsPossible.clear();

    for (auto sceneNode : mRenderablesToAddAsSoonAsPossible){
        addAndMakeRenderable(sceneNode); }
    mRenderablesToAddAsSoonAsPossible.clear();

    // draw
    draw();

    handleKeyboard();
    mContext.doneCurrent();
}