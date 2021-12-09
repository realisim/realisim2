#include "Rendering/Gpu/OpenGlHeaders.h"

#include <cassert>
#include "Core/Logger.h"
#include "Core/Path.h"
#include "DataStructures/Scene/ModelNode.h"
#include "Systems/Renderer/ModelRenderable.h"
#include "Systems/Renderer/RenderPasses/OpaquePass.h"



using namespace Realisim;
    using namespace Core;
    using namespace Math;
    using namespace Reactor;
    using namespace Rendering;

using namespace std;

//---------------------------------------------------------------------------------------------------------------------
OpaquePass::OpaquePass()
{}

//---------------------------------------------------------------------------------------------------------------------
OpaquePass::~OpaquePass()
{
}

//---------------------------------------------------------------------------------------------------------------------
void OpaquePass::applyGlState()
{
}


//---------------------------------------------------------------------------------------------------------------------
void OpaquePass::connectInputOutputs()
{
}

//---------------------------------------------------------------------------------------------------------------------
void OpaquePass::defineInputOutputs()
{
    Output colorOut("colorOut", mpFbo, fbaColor0);
    //output depthStencilOut("depthStencilOut", mpFbo, fbaDepthStencil);
    Output depthOutput("depthOut", mpFbo, fbaDepthOnly);
    
    addOutput(colorOut);
    //addOutput(depthStencilOut);
    addOutput(depthOutput);
}

//---------------------------------------------------------------------------------------------------------------------
void OpaquePass::initializeFbo()
{
    mpFbo = new FrameBufferObject(1, 1, 1);
    mFboIsOwned = true;

    // main color attachment
    Texture2d* pColorTexture = mpFbo->createAttachementTexture(
        Rendering::tifRgb8,
        Rendering::tfRgb,
        Rendering::dtUnsignedByte,
        Rendering::TextureFilter::tfLinear,
        Rendering::TextureWrapMode::twmClampToBorder,
        Rendering::TextureWrapMode::twmClampToBorder);

    // main depth attachement
    Texture2d* pDepthTexture = mpFbo->createAttachementTexture(
        Rendering::TextureInternalFormat::tifDepthInteger32,
        Rendering::TextureFormat::tfDepth,
        Rendering::DataType::dtUnsignedInteger,
        Rendering::TextureFilter::tfLinear,
        Rendering::TextureWrapMode::twmClampToBorder,
        Rendering::TextureWrapMode::twmClampToBorder);

    mpFbo->addAttachement(fbaColor0, pColorTexture);
    mpFbo->addAttachement(fbaDepthOnly, pDepthTexture);
    mpFbo->bake();

    assert(mpFbo->isValid());
}

//---------------------------------------------------------------------------------------------------------------------
void OpaquePass::loadShader(const std::string& iAssetPath) {
    LOG_TRACE(Logger::llNormal, "Loading opaquePass shader...");

    mShader.clear();

    //--- shaders   
    mShader.setName("opaquePassShader");
    mShader.addSourceFromFile(stVertex, Path::join(iAssetPath, "Shaders/model.vert"));
    mShader.addSourceFromFile(stFragment, Path::join(iAssetPath, "Shaders/modelWithMaterial.frag"));
    mShader.compile();
    mShader.link();

    if (mShader.hasErrors())
    {
        LOG_TRACE_ERROR(Logger::llNormal, "Error while loading: \n%s", mShader.getAndClearLastErrors().c_str());
    }
}

//---------------------------------------------------------------------------------------------------------------------
void OpaquePass::render(const Rendering::Camera& iCam, const std::map<uint32_t, IRenderable*> ipRenderables)
{
    mpFbo->push();
    mpFbo->drawTo(fbaColor0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Matrix4 modelMatrix(Vector3(0.0, 0.0, 0.0));

    glUseProgram(mShader.getProgramId());

    mShader.setUniform("uProjectionMatrix", iCam.getProjectionMatrix());
    mShader.setUniform("uViewMatrix", iCam.getViewMatrix());
    mShader.setUniform("uApplyLighting", true);
    mShader.setUniform("uUseSampler", false);
    mShader.setUniform("uLightPosition", Math::Vector3(-1, 0.7, 0.3));

    ModelRenderable* pModelRenderable = nullptr;
    for (auto itRenderable : ipRenderables) {
        pModelRenderable = (ModelRenderable *)itRenderable.second;

        ModelNode* pNode = pModelRenderable->getModelNode();
        mShader.setUniform("uModelMatrix", pNode->getWorldTransform());

        const Rendering::Texture2d* pDiffuseTex = pModelRenderable->getTexture();
        if (pDiffuseTex)
        {
            mShader.setUniform("uUseSampler", true);
            mShader.setUniform("uDiffuseSampler", 0);
            pDiffuseTex->bind(0);
        }
        else {
            mShader.setUniform("uUseSampler", false);
        }

        pModelRenderable->draw();

        if (pDiffuseTex){
            pDiffuseTex->unbind(); }
    }

    glUseProgram(0);

    mpFbo->pop();
}


//---------------------------------------------------------------------------------------------------------------------
void OpaquePass::revertGlState()
{
}

//---------------------------------------------------------------------------------------------------------------------
void sharePasses(const std::map<int, IRenderPass*> ipRenderPassNameToRenderPass)
{
    ;
}

