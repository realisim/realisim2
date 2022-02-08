#pragma once

#include "Systems/Renderer/IRenderable.h"
#include "Systems/Renderer/RenderPasses/IRenderPass.h"

namespace Realisim
{
namespace Reactor
{
    class PickingPass : public IRenderPass
    {
    public:
        PickingPass();
        PickingPass(const PickingPass&) = delete;
        PickingPass& operator=(const PickingPass&) = delete;
        virtual ~PickingPass();

        uint32_t pick(int iPixelX, int iPixelY);

    protected:
        virtual void loadShader(const std::string& iAssetPath) override final;
        virtual void sharePasses(const std::map<int, IRenderPass*>& iRenderPassNameToRenderPass) override final;
        virtual void defineInputOutputs() override final;
        virtual void applyGlState() final;
        virtual void render(const Rendering::Camera&, const std::vector<IRenderable*> iRenderables) final;
        virtual void revertGlState() final;

        Rendering::FrameBufferObject* mpOpaquePassFbo; //not owned
        Rendering::FrameBufferAttachementType mPickingFba;
        IRenderPass::Output mTextureOut;
    };

}
}