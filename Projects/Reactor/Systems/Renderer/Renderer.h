#pragma once

#include "Math/VectorI.h"
#include "Rendering/Gpu/Context.h"
#include "Rendering/Gpu/Device.h"
#include "Systems/ISystem.h"
#include "Systems/Renderer/RenderPasses/IRenderPass.h"

//--- temporary while no render passes.
#include "Rendering/Gpu/Shader.h"
#include "Rendering/Gpu/VertexArrayObject.h"

namespace Realisim
{
namespace Reactor
{
    class Broker;
    class Hub;

    class ImageNode;

    class IRenderable;
    class TextureRenderable;

    class Renderer : public ISystem {
    public:
        Renderer(Broker* ipBroker, Hub* ipHub);
        virtual ~Renderer();

        // do not change the order of this enum, as it drives the rendering order.
        enum RenderPassId {rpnPreDepth = 0, rpnOpaque, rpnScreenBlit, rpnUserDefined = 10000};

        void addRenderPass(int iRenderPassIndex, IRenderPass *ipPass);
        void addRenderPass(RenderPassId iIndex, IRenderPass* ipPass);

        void clear();
        // clearRenderPasses();

        virtual bool init() override final;

        virtual void preUpdate() override final;
        virtual void update() override final;

        void resizeGl(int iWidth, int iHeight);
        //removeRenderPass(int iRenderPassIndex);
        void setScene(Scene *ipScene);
        void setNativeWindowsGlContext(HDC iHDC, HGLRC iHGLRC);
        void swapBuffers();

    protected:
        void addDrawable(ThreeD::SceneNode* ipNode, IRenderable* ipRenderable); // renommer a addDrawable
        void addTextureRenderable(ImageNode* ipNode, TextureRenderable* ipRenderable); // renommer a addDrawable
        bool initializeGl();
        void initializePasses();
        void initializeSceneNode(ThreeD::SceneNode* ipNode);
        void draw();
        void handleKeyboard();
        void reloadShaders();
        void makeAndAddRenderable(ThreeD::SceneNode* ipNode);

        Rendering::Device mDevice;
        Rendering::Context mContext;

        //--- temp while waiting for SceneSystem
        Scene* mpScene;   // not owned

        //-- Data
        std::map<uint32_t, ThreeD::SceneNode*> mIdToSceneNode; // not owned
        std::map<uint32_t, IRenderable*> mIdToRenderable; // all renderables ever created, owned
        std::map<uint32_t, IRenderable*> mIdToDrawable; //  all drawables, not owned
        std::map<uint32_t, TextureRenderable*> mIdToTexture; // all textures not owned
        std::map<int, IRenderPass*> mRenderPassIdToRenderPassPtr; // owned
        std::vector<IRenderPass*> mRenderPasses;
    };

}

}