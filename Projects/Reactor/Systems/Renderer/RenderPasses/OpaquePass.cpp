#include "Rendering/Gpu/OpenGlHeaders.h"

#include <cassert>
#include "Core/Logger.h"
#include "Core/Path.h"
#include "DataStructures/Scene/ModelNode.h"
#include "Systems/Renderer/ModelRenderable.h"
#include "Systems/Renderer/RenderPasses/OpaquePass.h"
#include "Systems/Renderer/RenderPasses/RenderPassId.h"



using namespace Realisim;
    using namespace Core;
    using namespace Math;
    using namespace Reactor;
    using namespace Rendering;
    using namespace ThreeD;

using namespace std;

//---------------------------------------------------------------------------------------------------------------------
OpaquePass::OpaquePass() : IRenderPass(rpiOpaque)
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
void OpaquePass::defineInputOutputs()
{
    Output colorOut("colorOut", mpFbo, fbaColor0);
    Output depthOutput("depthOut", mpFbo, fbaDepthOnly);
    
    addOutput(colorOut);
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

    const string vertPath = Path::join(iAssetPath, "Shaders/model.vert");
    const string fragPath = Path::join(iAssetPath, "Shaders/modelWithMaterial.frag");
    IRenderPass::compileAndLinkShader(&mShader, "opaquePassShader", vertPath, fragPath);
}

//---------------------------------------------------------------------------------------------------------------------
void OpaquePass::render(const Rendering::Camera& iCam, const std::vector<IRenderable*> iRenderables)
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
    mShader.setUniformAsFloat("uAmbientFactor", getScene()->getAmbientFactor());

    ModelRenderable* pModelRenderable = nullptr;
    for (int i = 0; i < iRenderables.size(); ++i) {
        int samplerIndex = 0;
        pModelRenderable = (ModelRenderable *)iRenderables[i];

        ModelNode* pNode = pModelRenderable->getModelNode();
        mShader.setUniform("uModelMatrix", pNode->getWorldTransform());
        mShader.setUniformAsFloat("uTextureScaling", pNode->getTextureScaling());

        const Rendering::Texture2d* pDiffuseTex = pModelRenderable->getTexture(Material::ilDiffuse);
        if (pDiffuseTex)
        {
            mShader.setUniform("uUseSampler", true);
            mShader.setUniform("uDiffuseSampler", samplerIndex++);
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


