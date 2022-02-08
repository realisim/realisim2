#include "Rendering/Gpu/OpenGlHeaders.h"

#include <cassert>
#include "Core/Logger.h"
#include "Core/Path.h"
#include "Systems/Renderer/Renderer.h"
#include "Systems/Renderer/RenderPasses/CompositingPass.h"



using namespace Realisim;
    using namespace Core;
    using namespace Math;
    using namespace Reactor;
    using namespace Rendering;

using namespace std;

//---------------------------------------------------------------------------------------------------------------------
CompositingPass::CompositingPass() : IRenderPass(rpiCompositing),
mAdditionResultFba(fbaUnknown)
{}

//---------------------------------------------------------------------------------------------------------------------
CompositingPass::~CompositingPass()
{
}

//---------------------------------------------------------------------------------------------------------------------
void CompositingPass::applyGlState()
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
}

//---------------------------------------------------------------------------------------------------------------------
void CompositingPass::defineInputOutputs()
{
    Input samplerBaseInput("samplerBase", &mShader, "uSamplerBase");
    Input samplerToAdd0("samplerToAdd0", &mShader, "uSamplerToAdd0");
    Input samplerToAdd1("samplerToAdd1", &mShader, "uSamplerToAdd1");
    Output composedOut("composedOut", mpFbo, mAdditionResultFba);

    addInput(samplerBaseInput);
    addInput(samplerToAdd0);
    addInput(samplerToAdd1);
    addOutput(composedOut);
}

//---------------------------------------------------------------------------------------------------------------------
void CompositingPass::loadShader(const std::string& iAssetPath)
{ 
    LOG_TRACE(Logger::llNormal, "Loading CompositingPass shader...");

    const string vertPath = Path::join(iAssetPath, "Shaders/CompositingPass.vert");
    const string fragPath = Path::join(iAssetPath, "Shaders/CompositingPass.frag");
    IRenderPass::compileAndLinkShader(&mShader, "CompositingPassShader", vertPath, fragPath);
}

//---------------------------------------------------------------------------------------------------------------------
void CompositingPass::releaseGlRessources()
{
    IRenderPass::releaseGlRessources();
    mFullScreenQuad.clear();
}

//---------------------------------------------------------------------------------------------------------------------
void CompositingPass::render(const Rendering::Camera& iCam, const std::vector<IRenderable*> iRenderables)
{
    (void)iCam;
    (void)iRenderables;

    mpFbo->push();
    mpFbo->drawTo(mAdditionResultFba);

    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(mShader.getProgramId());

    mShader.setUniform("uProjectionMatrix", mFullScreen2dCam.getProjectionMatrix());

    bindConnections();
    mFullScreenQuad.draw();
    unbindConnections();

    glUseProgram(0);

    mpFbo->pop();

}

//---------------------------------------------------------------------------------------------------------------------
void CompositingPass::revertGlState()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

//---------------------------------------------------------------------------------------------------------------------
void CompositingPass::sharePasses(const std::map<int, IRenderPass*>& ipRenderPassNameToRenderPass)
{
    (void)ipRenderPassNameToRenderPass;
    IRenderPass::makeFullScreenQuadAnd2dCamera(&mFullScreenQuad, &mFullScreen2dCam);

    // get the opaque pass
    auto itOpaquePass = ipRenderPassNameToRenderPass.find(rpiOpaque);
    if (itOpaquePass == ipRenderPassNameToRenderPass.end())
    {
        LOG_TRACE_ERROR(Core::Logger::llNormal, "Could not find the Opaque pass...");
    } else {
        mpFbo = &itOpaquePass->second->getFboRef();

        // add 1 texture attachments to add two textures
        mAdditionResultFba = mpFbo->getNextAvailableColorAttachement();
        if (mAdditionResultFba != fbaUnknown)
        {
            Texture2d* pAdditionResultTex = mpFbo->createAttachementTexture(
                Rendering::tifRgb8,
                Rendering::tfRgb,
                Rendering::dtUnsignedByte,
                Rendering::TextureFilter::tfLinear,
                Rendering::TextureWrapMode::twmClampToBorder,
                Rendering::TextureWrapMode::twmClampToBorder);

            mpFbo->addAttachement(mAdditionResultFba, pAdditionResultTex);
            mpFbo->bake();
        }
        else {
            LOG_TRACE_ERROR(Logger::llNormal, "Opaque pass fbo has no available color attachment.");
        }
    }
}

