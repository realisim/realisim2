#include "Rendering/Gpu/OpenGlHeaders.h"

#include <cassert>
#include "Core/Logger.h"
#include "Core/Path.h"
#include "DataStructures/Scene/ModelNode.h"
#include "Systems/Renderer/Renderer.h"
#include "Systems/Renderer/ModelRenderable.h"
#include "Systems/Renderer/RenderPasses/GlowPass.h"



using namespace Realisim;
    using namespace Core;
    using namespace Math;
    using namespace Reactor;
    using namespace Rendering;
    using namespace ThreeD;

using namespace std;

//---------------------------------------------------------------------------------------------------------------------
GlowPass::GlowPass() : IRenderPass(rpiGlow),
    mpOpaquePassFbo(nullptr),
    mGlowTexturedModelFba(fbaUnknown)
{}

//---------------------------------------------------------------------------------------------------------------------
GlowPass::~GlowPass()
{
}

//---------------------------------------------------------------------------------------------------------------------
void GlowPass::applyBlur() {
    
    mpFbo->push();
    glUseProgram(mBlurShader.getProgramId());

    mBlurShader.setUniform("uProjectionMatrix", mFullScreen2dCam.getProjectionMatrix());
    mBlurShader.setUniform("uHorizontal", true);
    mBlurShader.setUniform("uInput", 0);
    //mBlurShader.setUniform("uKernel", 2);

    const Texture2d& toBlurHTex = mOpaqueGlowTexturedOut.mpFbo->getAttachement(mOpaqueGlowTexturedOut.mAttachment);

    mpFbo->drawTo({ fbaColor0 });
    glClear(GL_COLOR_BUFFER_BIT);
    
    toBlurHTex.bind(0);
    mFullScreenQuad.draw();
    toBlurHTex.unbind();

    const Texture2d& toBlurVTex = mpFbo->getAttachement(fbaColor0);
    mpFbo->drawTo({ fbaColor1 });
    glClear(GL_COLOR_BUFFER_BIT);

    toBlurVTex.bind(0);
    mBlurShader.setUniform("uHorizontal", false);
    mFullScreenQuad.draw();
    toBlurVTex.unbind();
    

    glUseProgram(0);
    mpFbo->pop();
}

//---------------------------------------------------------------------------------------------------------------------
void GlowPass::applyGlState()
{
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_FALSE);
}

//---------------------------------------------------------------------------------------------------------------------
void GlowPass::defineInputOutputs()
{
    // output for final blurred attachment.
    Output glow0Out("glow0Out", mpFbo, fbaColor0);
    Output glow1Out("glow1Out", mpFbo, fbaColor1);

    // add new output to opaque pass
    mOpaqueGlowTexturedOut = Output("opaqueGlowTexturedOut", mpOpaquePassFbo, mGlowTexturedModelFba);

    addOutput(mOpaqueGlowTexturedOut);
    addOutput(glow0Out);
    addOutput(glow1Out);
}

//---------------------------------------------------------------------------------------------------------------------
void GlowPass::initializeFbo()
{
    IRenderPass::makeFullScreenQuadAnd2dCamera(&mFullScreenQuad, &mFullScreen2dCam);

    mpFbo = new FrameBufferObject(1, 1, 1);
    mFboIsOwned = true;

    // horizontal blur
    Texture2d* pHorizontalBlur = mpFbo->createAttachementTexture(
        Rendering::tifRgb8,
        Rendering::tfRgb,
        Rendering::dtUnsignedByte,
        Rendering::TextureFilter::tfLinear,
        Rendering::TextureWrapMode::twmClampToBorder,
        Rendering::TextureWrapMode::twmClampToBorder);

    // vertical and final blur
    Texture2d* pFinalBlur = mpFbo->createAttachementTexture(
        Rendering::tifRgb8,
        Rendering::tfRgb,
        Rendering::dtUnsignedByte,
        Rendering::TextureFilter::tfLinear,
        Rendering::TextureWrapMode::twmClampToBorder,
        Rendering::TextureWrapMode::twmClampToBorder);

    mpFbo->addAttachement(fbaColor0, pHorizontalBlur);
    mpFbo->addAttachement(fbaColor1, pFinalBlur);
    mpFbo->bake();

    assert(mpFbo->isValid());
}

//---------------------------------------------------------------------------------------------------------------------
void GlowPass::loadShader(const std::string& iAssetPath) {
    LOG_TRACE(Logger::llNormal, "Loading GlowPass shader...");

    string vertPath = Path::join(iAssetPath, "Shaders/GlowPass.vert");
    string fragPath = Path::join(iAssetPath, "Shaders/GlowPass.frag");
    IRenderPass::compileAndLinkShader(&mShader, "GlowPassShader", vertPath, fragPath);

    vertPath = Path::join(iAssetPath, "Shaders/Blur2d.vert");
    fragPath = Path::join(iAssetPath, "Shaders/Blur2d.frag");
    IRenderPass::compileAndLinkShader(&mBlurShader, "GlowPassBlur2dShader", vertPath, fragPath);
}

//---------------------------------------------------------------------------------------------------------------------
void GlowPass::render(const Rendering::Camera& iCam, const std::vector<IRenderable*> ipRenderables)
{
    (void)iCam;
    (void)ipRenderables;

    // render to glow texture attachment
    renderGlowingModels(iCam, ipRenderables);

    // apply blur on mpGlowFbo (2 pass)
    applyBlur();
}

//---------------------------------------------------------------------------------------------------------------------
void GlowPass::renderGlowingModels(const Rendering::Camera& iCam, const std::vector<IRenderable*> iRenderables)
{
    mpOpaquePassFbo->push();
    mpOpaquePassFbo->drawTo(mGlowTexturedModelFba);

    glClear(GL_COLOR_BUFFER_BIT); // do not clear depth has it was already written by the opaque pass

    glUseProgram(mShader.getProgramId());

    mShader.setUniform("uProjectionMatrix", iCam.getProjectionMatrix());
    mShader.setUniform("uViewMatrix", iCam.getViewMatrix());
    
    ModelRenderable* pModelRenderable = nullptr;
    for (int i = 0; i < iRenderables.size(); ++i) {
        int samplerIndex = 0;
        pModelRenderable = (ModelRenderable*)iRenderables[i];

        ModelNode* pNode = pModelRenderable->getModelNode();
        mShader.setUniform("uModelMatrix", pNode->getWorldTransform());
        mShader.setUniformAsFloat("uTextureScaling", pNode->getTextureScaling());

        const Rendering::Texture2d* pGlowTex = pModelRenderable->getTexture(Material::ilAdditional0);
        if (pGlowTex)
        {
            mShader.setUniform("uUseSampler", true);
            mShader.setUniform("uGlowSampler", samplerIndex++);
            pGlowTex->bind(0);
        }
        else {
            mShader.setUniform("uUseSampler", false);
        }

        pModelRenderable->draw();

        if (pGlowTex){
            pGlowTex->unbind(); }
    }

    glUseProgram(0);

    mpOpaquePassFbo->pop();
}

//---------------------------------------------------------------------------------------------------------------------
void GlowPass::revertGlState()
{
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
}

//---------------------------------------------------------------------------------------------------------------------
void GlowPass::sharePasses(const std::map<int, IRenderPass*>& iRenderPassNameToRenderPass)
{
    // grab the opaque pass FBO;
    auto itOpaque = iRenderPassNameToRenderPass.find(rpiOpaque);

    IRenderPass* pOpaquePass = nullptr;
    assert(itOpaque != iRenderPassNameToRenderPass.end());
    if (itOpaque == iRenderPassNameToRenderPass.end()) {
        LOG_TRACE_ERROR(Logger::llNormal, "Could not find the opaque pass...");
    }
    else {
        pOpaquePass = itOpaque->second;
        mpOpaquePassFbo = &pOpaquePass->getFboRef();
    }

    if (mpOpaquePassFbo)
    {
        // add a color attachment for the glow textured pass.
        mGlowTexturedModelFba = mpOpaquePassFbo->getNextAvailableColorAttachement();
        if (mGlowTexturedModelFba != fbaUnknown)
        {
            Texture2d* pGlowTexture = mpOpaquePassFbo->createAttachementTexture(
                Rendering::tifRgb8,
                Rendering::tfRgb,
                Rendering::dtUnsignedByte,
                Rendering::TextureFilter::tfLinear,
                Rendering::TextureWrapMode::twmClampToBorder,
                Rendering::TextureWrapMode::twmClampToBorder);
            mpOpaquePassFbo->addAttachement(mGlowTexturedModelFba, pGlowTexture);
            mpOpaquePassFbo->bake();
        }
        else {
            LOG_TRACE_ERROR(Logger::llNormal, "Opaque pass fbo has no available color attachment.");
        }
    }
    
}

