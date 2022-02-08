#include "Rendering/Gpu/OpenGlHeaders.h"

#include <cassert>
#include "Core/Logger.h"
#include "Core/Path.h"
#include "DataStructures/Scene/ModelNode.h"
#include "Systems/Renderer/ModelRenderable.h"
#include "Systems/Renderer/RenderPasses/PickingPass.h"
#include "Systems/Renderer/RenderPasses/RenderPassId.h"



using namespace Realisim;
    using namespace Core;
    using namespace Math;
    using namespace Reactor;
    using namespace Rendering;
    using namespace ThreeD;

using namespace std;

//---------------------------------------------------------------------------------------------------------------------
PickingPass::PickingPass() : IRenderPass(rpiPicking),
mpOpaquePassFbo(nullptr),
mPickingFba(fbaUnknown)
{}

//---------------------------------------------------------------------------------------------------------------------
PickingPass::~PickingPass()
{
}

//---------------------------------------------------------------------------------------------------------------------
void PickingPass::applyGlState()
{
    // enable writing with glDepth equal, since we are drawing some opaque models to a different color attachment
    // using the same depth buffer as the one from the opaque pass
    //
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // this is the invalid id 0xFFFFFFFF
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_FALSE);
}

//---------------------------------------------------------------------------------------------------------------------
void PickingPass::defineInputOutputs()
{
    mTextureOut = Output("PickingPassTextureOut", mpOpaquePassFbo, mPickingFba);

    addOutput(mTextureOut);
}

//---------------------------------------------------------------------------------------------------------------------
void PickingPass::loadShader(const std::string& iAssetPath) {
    LOG_TRACE(Logger::llNormal, "Loading PickingPass shader...");

    const string vertPath = Path::join(iAssetPath, "Shaders/PickingPass.vert");
    const string fragPath = Path::join(iAssetPath, "Shaders/PickingPass.frag");
    IRenderPass::compileAndLinkShader(&mShader, "PickingPassShader", vertPath, fragPath);
}

//---------------------------------------------------------------------------------------------------------------------
uint32_t PickingPass::pick(int iPixelX, int iPixelY)
{
    uint32_t r = SceneNode::mInvalidId;

    mpOpaquePassFbo->push();
    mpOpaquePassFbo->readFrom(mPickingFba);

    uint8_t dataBuf[1 * 1 * 4];
    glReadPixels(iPixelX, iPixelY, 1, 1, toGlTextureFormat(tfRgba), toGlDataType(dtUnsignedByte), &dataBuf[0]);

    r = (uint32_t)(dataBuf[0] << 24) +
        (uint32_t)(dataBuf[1] << 16) +
        (uint32_t)(dataBuf[2] << 8) +
        (uint32_t)(dataBuf[3]);

    mpOpaquePassFbo->pop();

    return r;
}

//---------------------------------------------------------------------------------------------------------------------
void PickingPass::render(const Rendering::Camera& iCam, const std::vector<IRenderable*> iRenderables)
{
    mpOpaquePassFbo->push();
    mpOpaquePassFbo->drawTo(mPickingFba);

    glClear(GL_COLOR_BUFFER_BIT); // do not clear depth has it was already written by the opaque pass

    glUseProgram(mShader.getProgramId());

    mShader.setUniform("uProjectionMatrix", iCam.getProjectionMatrix());
    mShader.setUniform("uViewMatrix", iCam.getViewMatrix());

    ModelRenderable* pModelRenderable = nullptr;
    for (int i = 0; i < iRenderables.size(); ++i) {
        pModelRenderable = (ModelRenderable*)iRenderables[i];

        ModelNode* pNode = pModelRenderable->getModelNode();
        const uint32_t nodeId = pNode->getId();
        Core::ColorRgbaUint8 idAsColor(
            (uint8_t)((nodeId & 0xFF000000) >> 24),
            (uint8_t)((nodeId & 0x00FF0000) >> 16),
            (uint8_t)((nodeId & 0x0000FF00) >> 8),
            (uint8_t)((nodeId & 0x000000FF)) );

        mShader.setUniform("uModelMatrix", pNode->getWorldTransform());
        mShader.setUniform("uIdAsColor", idAsColor);

        pModelRenderable->draw();
    }

    glUseProgram(0);

    mpOpaquePassFbo->pop();
}


//---------------------------------------------------------------------------------------------------------------------
void PickingPass::revertGlState()
{
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

//---------------------------------------------------------------------------------------------------------------------
void PickingPass::sharePasses(const std::map<int, IRenderPass*>& ipRenderPassNameToRenderPass)
{
    // get the opaque pass, because we need the global depth buffer to properly do the picking
    auto itOpaquePass = ipRenderPassNameToRenderPass.find(rpiOpaque);
    if (itOpaquePass == ipRenderPassNameToRenderPass.end())
    {
        LOG_TRACE_ERROR(Core::Logger::llNormal, "Could not find the Opaque pass...");
    }
    else {
        mpOpaquePassFbo = &itOpaquePass->second->getFboRef();

        // add 1 texture attachments to add two textures
        mPickingFba = mpOpaquePassFbo->getNextAvailableColorAttachement();
        if (mPickingFba != fbaUnknown)
        {
            //since we will use the id of the object as it'S color, make sure
            // we have TextureFilter::tfNearest so the id (color) is never smudged around
            //
            Texture2d* pPickingTex = mpOpaquePassFbo->createAttachementTexture(
                Rendering::tifRgba8,
                Rendering::tfRgba,
                Rendering::dtUnsignedByte,
                Rendering::TextureFilter::tfNearest,
                Rendering::TextureWrapMode::twmClampToBorder,
                Rendering::TextureWrapMode::twmClampToBorder);

            mpOpaquePassFbo->addAttachement(mPickingFba, pPickingTex);
            mpOpaquePassFbo->bake();
        }
        else {
            LOG_TRACE_ERROR(Logger::llNormal, "Opaque pass fbo has no available color attachment.");
        }
    }
}


