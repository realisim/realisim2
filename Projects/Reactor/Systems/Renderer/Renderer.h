#pragma once

#include "Math/VectorI.h"
#include "Rendering/Gpu/Context.h"
#include "Rendering/Gpu/Device.h"
#include "Systems/ISystem.h"
#include "Systems/Renderer/IRenderable.h"

//--- temporary while no render passes.
#include "Rendering/Gpu/Shader.h"
#include "Rendering/Gpu/VertexArrayObject.h"

namespace Realisim
{
namespace Reactor
{
    class Broker;
    class Hub;

    class Renderer : public ISystem {
    public:
        Renderer(Broker* ipBroker, Hub* ipHub);
        virtual ~Renderer();

        void clear();

        virtual bool init() override final;

        virtual void preUpdate() override final;
        virtual void update() override final;

        void resizeGl(int iWidth, int iHeight);
        void setScene(Scene *ipScene);
        void setNativeWindowsGlContext(HDC iHDC, HGLRC iHGLRC);
        void swapBuffers();

    protected:
        void addRenderable(ThreeD::SceneNode* ipNode, IRenderable* ipRenderable);
        bool initializeGl();
        void initializeSceneNode(ThreeD::SceneNode* ipNode);
        void draw();
        void handleKeyboard();
        void loadShaders();
        void makeAndAddRenderable(ThreeD::SceneNode* ipNode);

        Rendering::Device mDevice;
        Rendering::Context mContext;

        //--- temp while waiting for SceneSystem
        Scene* mpScene;   // not owned

        //--- temporary while waiting for render passes
        Rendering::Shader mModelShader;
        Rendering::VertexArrayObject* mpVbo; //owned

        //-- Data
        std::map<uint32_t, ThreeD::SceneNode*> mIdToSceneNode;
        std::map<uint32_t, IRenderable*> mIdToRenderable;
    };

}

}