#pragma once


#include "DataStructures/Scene/Scene.h"
#include "Rendering/Camera.h"
#include "Rendering/Gpu/DataType.h"
#include "Rendering/Gpu/FrameBufferObject.h"
#include "Rendering/Gpu/VertexArrayObject.h"
#include "Rendering/Gpu/Shader.h"
#include "Systems/Renderer/RenderPasses/RenderPassId.h"
#include <string>

namespace Realisim
{
namespace Reactor
{
    class IRenderable;

    class IRenderPass
    {
    public:
        friend class Renderer; // to trigger initialization and pass bindings
        IRenderPass() = delete;
        IRenderPass(int iId);
        IRenderPass(const IRenderPass&) = delete;
        IRenderPass& operator=(const IRenderPass) = delete;
        virtual ~IRenderPass() = 0;


        struct Input {
            Input() : mName("noName"), mpShader(nullptr), mUniformName("noName") {}
            Input(const std::string& iName, Rendering::Shader* ipShader, const std::string& iUniformName) : mName(iName), mpShader(ipShader), mUniformName(iUniformName) { ; }
            std::string mName;
            Rendering::Shader* mpShader; // not owned
            std::string mUniformName;
        };

        struct Output {
            Output() : mName("noName"), mpFbo(nullptr), mAttachment(Rendering::fbaUnknown) {}
            Output(const std::string& iName, Rendering::FrameBufferObject* ipFbo, Rendering::FrameBufferAttachementType iFba) : mName(iName), mpFbo(ipFbo), mAttachment(iFba) { ; }
            std::string mName;
            Rendering::FrameBufferObject* mpFbo; // not owned
            Rendering::FrameBufferAttachementType mAttachment;
        };

        struct Connection {
            Connection() : mInputIndex(-1) { ; }
            Output mOutput;
            int mInputIndex;
        };


        void addInput(const Input& iInput);
        void addOutput(const Output& iOutput);
        void connect(IRenderPass *ipParentPass, const std::string& iOutputName, const std::string& iInputName);
        static void compileAndLinkShader(Rendering::Shader* ipShader, const std::string& iName, const std::string& iVertPath, const std::string& iFragPath);
        virtual void clear();
        int findInput(const std::string& iName) const;
        int findOutput(const std::string& iName) const;
        int getId() const { return mId; }
        const Input& getInput(int iIndex) const;
        Input& getInputRef(int iIndex);
        const Rendering::FrameBufferObject& getFboRef() const;
        Rendering::FrameBufferObject& getFboRef();
        const Output& getOutput(int iIndex) const;
        Output& getOutputRef(int iIndex);
        const Output& getOutput(const std::string& iName) const;
        Output& getOutputRef(const std::string& iName);
        const std::string& getName() const { return mName; }
        int getNumberOfInputs() const { return (int)mInputs.size(); }
        int getNumberOfOutputs() const { return (int)mOutputs.size(); }
        const Scene* getScene() const { return mpScene; }
        bool isFboOwned() const { return mFboIsOwned; }

        static void makeFullScreenQuadAnd2dCamera(Rendering::VertexArrayObject* pVao, Rendering::Camera* pCam);
        void resize(int iWidth, int iHeight);        

        void removeInput(int iIndex);
        void removeOutput(int iIndex);
        void setName(const std::string& iV) { mName = iV; }
        void setScene(const Scene* ipScene) { mpScene = ipScene; }

    protected:
        void bindConnections();
        virtual void initializeFbo() { ; }
        virtual void loadShader(const std::string& iAssetPath) { (void)iAssetPath; }
        virtual void sharePasses(const std::map<int, IRenderPass*>& iRenderPassNameToRenderPass) { (void)iRenderPassNameToRenderPass; }
        virtual void defineInputOutputs() = 0;
        virtual void applyGlState() = 0;
        virtual void render(const Rendering::Camera&, const std::vector<IRenderable*> iRenderables) = 0;
        virtual void revertGlState() = 0;
        virtual void releaseGlRessources();
        void unbindConnections();

        static Input mDummyInput;
        static Output mDummyOutput;
        static Rendering::FrameBufferObject mDummyFbo;

        int mId;

        std::string mName;
        std::vector<Input> mInputs;
        std::vector<Output> mOutputs;
        std::vector<Connection> mConnections;

        Rendering::Shader mShader;
        Rendering::FrameBufferObject* mpFbo; // each pass decides if it owns of share an Fbo
        bool mFboIsOwned;

        const Scene* mpScene; // not owned, should never be null
    };

}
}

