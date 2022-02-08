#pragma once

#include "Math/VectorI.h"
#include "Rendering/Gpu/Context.h"
#include "Rendering/Gpu/Device.h"
#include "Systems/ISystem.h"
#include "Systems/Renderer/RenderPasses/IRenderPass.h"
#include "Systems/Renderer/RenderPasses/RenderPassId.h"

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

        void addAndMakeRenderableAsSoonAsPossible(ThreeD::SceneNode* ipNode);
        void addRenderPass(int iRenderPassIndex, IRenderPass *ipPass);
        void addRenderPass(RenderPassId iIndex, IRenderPass* ipPass);
        void addToRenderPass(uint32_t iSceneNodeId, int iRenderPassId);

        void clear();
        // clearRenderPasses();

        virtual bool init() override final;
        void initializePasses();

        uint32_t pick(int iPixelX, int iPixelY);
        virtual void preUpdate() override final;
        virtual void update() override final;

        const ThreeD::SceneNode* findNode(uint32_t iNodeId) const;
        ThreeD::SceneNode* findNodePtr(uint32_t iNodeId);
        void removeFromRenderPass(uint32_t iSceneNodeId, int iRenderPassId);
        void removeRenderableAsSoonAsPossible(uint32_t iNodeId);
        void removeRenderableAsSoonAsPossible(const ThreeD::SceneNode* ipNode);
        void resizeGl(int iWidth, int iHeight);
        //removeRenderPass(int iRenderPassIndex);
        void setScene(Scene *ipScene);
        void setNativeWindowsGlContext(HDC iHDC, HGLRC iHGLRC);
        void swapBuffers();

    protected:
        void addAndMakeRenderable(ThreeD::SceneNode* ipNode);
        void addDrawable(ThreeD::SceneNode* ipNode, IRenderable* ipRenderable); // renommer a addDrawable
        void addTextureRenderable(ImageNode* ipNode, TextureRenderable* ipRenderable); // renommer a addDrawable
        void connectBuiltInPasses();
        bool initializeGl();
        void initializeSceneNode(ThreeD::SceneNode* ipNode);
        void draw();
        void handleKeyboard();
        void reloadShaders();
        void removeFromAllPasses(IRenderable* ipToRemove);
        void removeRenderable(int iNodeId);

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
        std::vector<IRenderPass*> mRenderPasses; // defines the draw order

        std::map<int, std::vector<IRenderable*>> mPassIdToDrawables; // the list of drawable per pass should be IDrawable

        std::vector<ThreeD::SceneNode*> mRenderablesToAddAsSoonAsPossible;
        std::vector<uint32_t> mRenderablesToRemoveAsSoonAsPossible;
    };

}

}