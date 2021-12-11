#include "Rendering/Gpu/OpenGlHeaders.h"

#include <cassert>
#include "Core/Logger.h"
#include "Core/Path.h"
#include "Systems/Renderer/Renderer.h"
#include "Systems/Renderer/RenderPasses/ScreenBlitPass.h"



using namespace Realisim;
    using namespace Core;
    using namespace Math;
    using namespace Reactor;
    using namespace Rendering;

using namespace std;

//---------------------------------------------------------------------------------------------------------------------
ScreenBlitPass::ScreenBlitPass() : IRenderPass(rpiScreenBlit),
    mpOutputToBlit(nullptr)
{}

//---------------------------------------------------------------------------------------------------------------------
ScreenBlitPass::~ScreenBlitPass()
{
}

//---------------------------------------------------------------------------------------------------------------------
void ScreenBlitPass::applyGlState()
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
}

//---------------------------------------------------------------------------------------------------------------------
void ScreenBlitPass::connectInputOutputs()
{
}

//---------------------------------------------------------------------------------------------------------------------
void ScreenBlitPass::defineInputOutputs()
{
    Input uInput;
    uInput.mName = "screenBlitInput";
    uInput.mpShader = &mShader;
    uInput.mUniformName = "uInput";
    
    addInput(uInput);
}

//---------------------------------------------------------------------------------------------------------------------
void ScreenBlitPass::loadShader(const std::string& iAssetPath)
{ 
    LOG_TRACE(Logger::llNormal, "Loading ScreenBlitPass shader...");

    const string vertPath = Path::join(iAssetPath, "Shaders/screenBlitPass.vert");
    const string fragPath = Path::join(iAssetPath, "Shaders/screenBlitPass.frag");
    IRenderPass::compileAndLinkShader(&mShader, "ScreenBlitPassShader", vertPath, fragPath);
}

//---------------------------------------------------------------------------------------------------------------------
void ScreenBlitPass::releaseGlRessources()
{
    IRenderPass::releaseGlRessources();
    mFullScreenQuad.clear();
}

//---------------------------------------------------------------------------------------------------------------------
void ScreenBlitPass::render(const Rendering::Camera& iCam, const std::vector<IRenderable*> iRenderables)
{
    (void)iCam;
    (void)iRenderables;

    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(mShader.getProgramId());

    mShader.setUniform("uProjectionMatrix", mFullScreen2dCam.getProjectionMatrix());
    mShader.setUniform("uInput", 0);

    const Texture2d &output0Tex = mpOutputToBlit->mpFbo->getAttachement(mpOutputToBlit->mAttachment);

    output0Tex.bind(0);
    mFullScreenQuad.draw();
    output0Tex.unbind();

    glUseProgram(0);

}

//---------------------------------------------------------------------------------------------------------------------
void ScreenBlitPass::revertGlState()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

//---------------------------------------------------------------------------------------------------------------------
void ScreenBlitPass::sharePasses(const std::map<int, IRenderPass*> ipRenderPassNameToRenderPass)
{
    IRenderPass::makeFullScreenQuadAnd2dCamera(&mFullScreenQuad, &mFullScreen2dCam);

    // get the opaque pass fbo
    auto itOpaque = ipRenderPassNameToRenderPass.find(rpiOpaque);
    auto itGlow = ipRenderPassNameToRenderPass.find(rpiGlow);

    assert(itOpaque != ipRenderPassNameToRenderPass.end() &&
        itGlow != ipRenderPassNameToRenderPass.end());
    if (itOpaque == ipRenderPassNameToRenderPass.end()) {
        LOG_TRACE_ERROR(Logger::llNormal, "Could not find the opaque pass...");
    }
    else if (itGlow == ipRenderPassNameToRenderPass.end()) {
        LOG_TRACE_ERROR(Logger::llNormal, "Could not find the glow pass...");
    }
    else {
        IRenderPass* pOpaquePass = itOpaque->second;
        //IRenderPass* pGlowPass = itGlow->second;

        mpOutputToBlit = &pOpaquePass->getOutput("colorOut");
        //mpOutputToBlit = &pOpaquePass->getOutput("glowTexturedOut");
        //mpOutputToBlit = &pGlowPass->getOutput("glow1Out");
    }
}

