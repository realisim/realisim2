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
ScreenBlitPass::ScreenBlitPass() :
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
void ScreenBlitPass::initFullScreenQuad()
{
    // init full screen 2d cam
    Projection proj;
    proj.setProjection(-1, 1, -1, 1, -10, 10, Projection::Type::tOrthogonal);
    mFullScreen2dCam.setProjection(proj);
    mFullScreen2dCam.set(Vector3(0.0, 0.0, 1.0), Vector3(0.0, 0.0, 0.0), Vector3(0.0, 1.0, 0.0));

    //init fullscreen quad
    BufferObject* pVboVertex, * pVboTex, * pVboIndices;
    pVboVertex = new BufferObject();
    pVboTex = new BufferObject();
    pVboIndices = new BufferObject();

    float vertices[8] = {
        -1.0, -1.0,
         1.0, -1.0,
         1.0,  1.0,
        -1.0,  1.0 };
    BufferObjectDataPlain* pVertexDataStructure = new BufferObjectDataPlain();
    pVertexDataStructure->addAttribute(Rendering::dtFloat, 2, BufferObject::lliVertex);
    pVboVertex->setDataStructure(pVertexDataStructure);
    pVboVertex->assignData(BufferBindingTarget::bbtArrayBuffer, BufferDataUsage::bduStaticDraw, 8, &vertices[0]);

    float texCoords[8] = {
        0.0, 0.0,
        1.0, 0.0,
        1.0, 1.0,
        0.0, 1.0 };
    BufferObjectDataPlain* pTexDataStructure = new BufferObjectDataPlain();
    pTexDataStructure->addAttribute(Rendering::dtFloat, 2, BufferObject::lliTexture);
    pVboTex->setDataStructure(pTexDataStructure);
    pVboTex->assignData(BufferBindingTarget::bbtArrayBuffer, BufferDataUsage::bduStaticDraw, 8, &texCoords[0]);

    uint16_t indices[6] = {
        0, 1, 2,
        0, 2, 3 };
    BufferObjectDataPlain* pIndicesDataStructure = new BufferObjectDataPlain();
    pIndicesDataStructure->addAttribute(Rendering::dtUnsignedShort, 1, 0);
    pVboIndices->setDataStructure(pIndicesDataStructure);
    pVboIndices->assignData(BufferBindingTarget::bbtElementArrayBuffer, BufferDataUsage::bduStaticDraw, 6, &indices[0]);

    mFullScreenQuad.addAndTakeOwnership(pVboVertex);
    mFullScreenQuad.addAndTakeOwnership(pVboTex);
    mFullScreenQuad.addAndTakeOwnership(pVboIndices);
    mFullScreenQuad.bake(DrawMode::dmTriangles);
}

//---------------------------------------------------------------------------------------------------------------------
void ScreenBlitPass::loadShader(const std::string& iAssetPath)
{ 
    LOG_TRACE(Logger::llNormal, "Loading ScreenBlitPass shader...");

    mShader.clear();

    //--- shaders   
    mShader.setName("ScreenBlitPassShader");
    mShader.addSourceFromFile(stVertex, Path::join(iAssetPath, "Shaders/screenBlitPass.vert"));
    mShader.addSourceFromFile(stFragment, Path::join(iAssetPath, "Shaders/screenBlitPass.frag"));
    mShader.compile();
    mShader.link();

    if (mShader.hasErrors())
    {
        LOG_TRACE_ERROR(Logger::llNormal, "Error while loading: \n%s", mShader.getAndClearLastErrors().c_str());
    }
}

//---------------------------------------------------------------------------------------------------------------------
void ScreenBlitPass::releaseGlRessources()
{
    IRenderPass::releaseGlRessources();
    mFullScreenQuad.clear();
}

//---------------------------------------------------------------------------------------------------------------------
void ScreenBlitPass::render(const Rendering::Camera& iCam, const std::map<uint32_t, IRenderable*> ipRenderables)
{
    (void)iCam;
    (void)ipRenderables;

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
    initFullScreenQuad();

    // get the opaque pass fbo
    auto itOpaque = ipRenderPassNameToRenderPass.find(Renderer::rpnOpaque);

    assert(itOpaque != ipRenderPassNameToRenderPass.end());
    if (itOpaque == ipRenderPassNameToRenderPass.end()) {
        LOG_TRACE_ERROR(Logger::llNormal, "Could not find the opaque pass...");
    } else {
        IRenderPass* pPass = itOpaque->second;

        mpOutputToBlit = &pPass->getOutput("colorOut");
    }
}

