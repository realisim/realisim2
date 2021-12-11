#pragma once

#include "Systems/Renderer/IRenderable.h"
#include "Systems/Renderer/RenderPasses/IRenderPass.h"

namespace Realisim
{
namespace Reactor
{
    class ScreenBlitPass : public IRenderPass
    {
    public:
        ScreenBlitPass();
        ScreenBlitPass(const ScreenBlitPass&) = delete;
        ScreenBlitPass& operator=(const ScreenBlitPass&) = delete;
        virtual ~ScreenBlitPass();

    protected:
        virtual void loadShader(const std::string& iAssetPath);
        virtual void sharePasses(const std::map<int, IRenderPass*> ipRenderPassNameToRenderPass);
        virtual void defineInputOutputs() override final;
        virtual void connectInputOutputs() override final;
        virtual void releaseGlRessources() override final;
        virtual void applyGlState() final;
        virtual void render(const Rendering::Camera&, const std::vector<IRenderable*> iRenderables) final;
        virtual void revertGlState() final;

        Rendering::Camera mFullScreen2dCam;
        Rendering::VertexArrayObject mFullScreenQuad;
        const Output* mpOutputToBlit;
    };

}
}