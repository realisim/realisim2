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
ScreenBlitPass::ScreenBlitPass() : IRenderPass(rpiScreenBlit)
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
void ScreenBlitPass::defineInputOutputs()
{
    Input uInput("screenBlitInput", &mShader, "uInput");
    
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

    bindConnections();
    mFullScreenQuad.draw();
    unbindConnections();

    glUseProgram(0);

}

//---------------------------------------------------------------------------------------------------------------------
void ScreenBlitPass::revertGlState()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

//---------------------------------------------------------------------------------------------------------------------
void ScreenBlitPass::sharePasses(const std::map<int, IRenderPass*>& iRenderPassNameToRenderPass)
{
    (void)iRenderPassNameToRenderPass;
    IRenderPass::makeFullScreenQuadAnd2dCamera(&mFullScreenQuad, &mFullScreen2dCam);
}

