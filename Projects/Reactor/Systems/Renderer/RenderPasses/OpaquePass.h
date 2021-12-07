#pragma once

#include "Systems/Renderer/IRenderable.h"
#include "Systems/Renderer/RenderPasses/IRenderPass.h"

namespace Realisim
{
namespace Reactor
{
    class OpaquePass : public IRenderPass
    {
    public:
        OpaquePass();
        OpaquePass(const OpaquePass&) = delete;
        OpaquePass& operator=(const OpaquePass&) = delete;
        virtual ~OpaquePass();

        

    protected:
        virtual void initializeFbo() override final;
        virtual void loadShader(const std::string& iAssetPath) override final;
        virtual void sharePasses(const std::map<int, IRenderPass*> ipRenderPassNameToRenderPass) { ; }
        virtual void defineInputOutputs() override final;
        virtual void connectInputOutputs() override final;
        virtual void applyGlState() final;
        virtual void render(const Rendering::Camera&, const std::map<uint32_t, IRenderable*> ipRenderables) final;
        virtual void revertGlState() final;
    };

}
}