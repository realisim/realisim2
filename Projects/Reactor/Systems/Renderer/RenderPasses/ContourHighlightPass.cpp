#include "Rendering/Gpu/OpenGlHeaders.h"

#include <cassert>
#include "Core/Color.h"
#include "Core/Logger.h"
#include "Core/Path.h"
#include "DataStructures/Scene/ModelNode.h"
#include "Systems/Renderer/Renderer.h"
#include "Systems/Renderer/ModelRenderable.h"
#include "Systems/Renderer/RenderPasses/ContourHighlightPass.h"



using namespace Realisim;
    using namespace Core;
    using namespace Math;
    using namespace Reactor;
    using namespace Rendering;
    using namespace ThreeD;

using namespace std;

//---------------------------------------------------------------------------------------------------------------------
ContourHighlightPass::ContourHighlightPass() : IRenderPass(rpiContourHighlight)
{}

//---------------------------------------------------------------------------------------------------------------------
ContourHighlightPass::~ContourHighlightPass()
{
}

//---------------------------------------------------------------------------------------------------------------------
void ContourHighlightPass::applyGlState()
{
    //glDisable(GL_DEPTH_TEST);
}

//---------------------------------------------------------------------------------------------------------------------
void ContourHighlightPass::defineInputOutputs()
{
    Output drawSelectedModelsOut("drawSelectedModelsOut", mpFbo, fbaColor0);
    Output countourHighlightOut("countourHighlightOut", mpFbo, fbaColor1);

    addOutput(drawSelectedModelsOut);
    addOutput(countourHighlightOut);
}

//---------------------------------------------------------------------------------------------------------------------
void ContourHighlightPass::initializeFbo()
{
    IRenderPass::makeFullScreenQuadAnd2dCamera(&mFullScreenQuad, &mFullScreen2dCam);

    mpFbo = new FrameBufferObject(1, 1, 1);
    mFboIsOwned = true;

    // draw selected models, whithout depth buffer and a single channel.
    Texture2d* pDrawSelectedModelsTex = mpFbo->createAttachementTexture(
        Rendering::tifR8,
        Rendering::tfRed,
        Rendering::dtUnsignedByte,
        Rendering::TextureFilter::tfNearest,
        Rendering::TextureWrapMode::twmClampToBorder,
        Rendering::TextureWrapMode::twmClampToBorder);

    // contour outline tex
    Texture2d* pContourTex = mpFbo->createAttachementTexture(
        Rendering::tifRgb8,
        Rendering::tfRgb,
        Rendering::dtUnsignedByte,
        Rendering::TextureFilter::tfLinear,
        Rendering::TextureWrapMode::twmClampToBorder,
        Rendering::TextureWrapMode::twmClampToBorder);

    mpFbo->addAttachement(fbaColor0, pDrawSelectedModelsTex);
    mpFbo->addAttachement(fbaColor1, pContourTex);
    mpFbo->bake();

    assert(mpFbo->isValid());
}

//---------------------------------------------------------------------------------------------------------------------
void ContourHighlightPass::loadShader(const std::string& iAssetPath) {
    LOG_TRACE(Logger::llNormal, "Loading ContourHighlightPass shader...");

    string vertPath = Path::join(iAssetPath, "Shaders/ContourHighlightPass_drawSelectedModels.vert");
    string fragPath = Path::join(iAssetPath, "Shaders/ContourHighlightPass_drawSelectedModels.frag");
    IRenderPass::compileAndLinkShader(&mDrawSelectedModelsShader, "ContourHighlightPass_drawSelectedModels", vertPath, fragPath);

    vertPath = Path::join(iAssetPath, "Shaders/ContourHighlightPass_drawContour.vert");
    fragPath = Path::join(iAssetPath, "Shaders/ContourHighlightPass_drawContour.frag");
    IRenderPass::compileAndLinkShader(&mDrawContourShader, "ContourHighlightPass_drawSelectedModels", vertPath, fragPath);
}

//---------------------------------------------------------------------------------------------------------------------
void ContourHighlightPass::render(const Rendering::Camera& iCam, const std::vector<IRenderable*> ipRenderables)
{
    mpFbo->push();
    renderSelectedModels(iCam, ipRenderables);
    renderContour();
    mpFbo->pop();
}

//---------------------------------------------------------------------------------------------------------------------
void ContourHighlightPass::renderContour()
{
    mpFbo->drawTo(fbaColor1);

    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(mDrawContourShader.getProgramId());

    mDrawContourShader.setUniform("uProjectionMatrix", mFullScreen2dCam.getProjectionMatrix());
    mDrawContourShader.setUniform("uInput", 0); // texture 0, which is the selectedContour
    mDrawContourShader.setUniform("uContourColor", Core::ColorRgbF32(0.1f, 0.8f, 0.1f)); // texture 0, which is the selectedContour

    const Texture2d& selectedModelsTex = mpFbo->getAttachement(fbaColor0);
    selectedModelsTex.bind(0);
    mFullScreenQuad.draw();
    selectedModelsTex.unbind();
   
    glUseProgram(0);
}

//---------------------------------------------------------------------------------------------------------------------
void ContourHighlightPass::renderSelectedModels(const Rendering::Camera& iCam, const std::vector<IRenderable*> iRenderables)
{
    mpFbo->drawTo(fbaColor0);

    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(mDrawSelectedModelsShader.getProgramId());

    mDrawSelectedModelsShader.setUniform("uProjectionMatrix", iCam.getProjectionMatrix());
    mDrawSelectedModelsShader.setUniform("uViewMatrix", iCam.getViewMatrix());
    
    ModelRenderable* pModelRenderable = nullptr;
    for (int i = 0; i < iRenderables.size(); ++i) {
        pModelRenderable = (ModelRenderable*)iRenderables[i];

        ModelNode* pNode = pModelRenderable->getModelNode();
        mDrawSelectedModelsShader.setUniform("uModelMatrix", pNode->getWorldTransform());        

        pModelRenderable->draw();
    }

    glUseProgram(0);

}

//---------------------------------------------------------------------------------------------------------------------
void ContourHighlightPass::revertGlState()
{
    //glEnable(GL_DEPTH_TEST);
}

////---------------------------------------------------------------------------------------------------------------------
//void ContourHighlightPass::sharePasses(const std::map<int, IRenderPass*>& iRenderPassNameToRenderPass)
//{
//    //// grab the opaque pass FBO;
//    //auto itOpaque = iRenderPassNameToRenderPass.find(rpiOpaque);
//
//    //IRenderPass* pOpaquePass = nullptr;
//    //assert(itOpaque != iRenderPassNameToRenderPass.end());
//    //if (itOpaque == iRenderPassNameToRenderPass.end()) {
//    //    LOG_TRACE_ERROR(Logger::llNormal, "Could not find the opaque pass...");
//    //}
//    //else {
//    //    pOpaquePass = itOpaque->second;
//    //    mpOpaquePassFbo = &pOpaquePass->getFboRef();
//    //}
//
//    //if (mpOpaquePassFbo)
//    //{
//    //    // add a color attachment for the glow textured pass.
//    //    mGlowTexturedModelFba = mpOpaquePassFbo->getNextAvailableColorAttachement();
//    //    if (mGlowTexturedModelFba != fbaUnknown)
//    //    {
//    //        Texture2d* pGlowTexture = mpOpaquePassFbo->createAttachementTexture(
//    //            Rendering::tifRgb8,
//    //            Rendering::tfRgb,
//    //            Rendering::dtUnsignedByte,
//    //            Rendering::TextureFilter::tfLinear,
//    //            Rendering::TextureWrapMode::twmClampToBorder,
//    //            Rendering::TextureWrapMode::twmClampToBorder);
//    //        mpOpaquePassFbo->addAttachement(mGlowTexturedModelFba, pGlowTexture);
//    //        mpOpaquePassFbo->bake();
//    //    }
//    //    else {
//    //        LOG_TRACE_ERROR(Logger::llNormal, "Opaque pass fbo has no available color attachment.");
//    //    }
//    //}
//    
//}

