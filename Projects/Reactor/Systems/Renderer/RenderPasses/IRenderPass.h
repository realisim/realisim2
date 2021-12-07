#pragma once


#include "Rendering/Camera.h"
#include "Rendering/Gpu/DataType.h"
#include "Rendering/Gpu/FrameBufferObject.h"
#include "Rendering/Gpu/Shader.h"
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

        IRenderPass();
        IRenderPass(const IRenderPass&) = delete;
        IRenderPass& operator=(const IRenderPass) = delete;
        virtual ~IRenderPass() = 0;


        struct Input {
            Input() : mName("noName"), mpShader(nullptr), mUniformName("noName") {}
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
            Connection() : mOutputIndex(-1), mInputIndex(-1) { ; }
            Connection(int iO, int iI) : mOutputIndex(iO), mInputIndex(iI) { ; }
            int mOutputIndex;
            int mInputIndex;
        };


        void addInput(const Input& iInput);
        void addOutput(const Output& iOutput);
        void connect(IRenderPass *ipParentPass, const std::string& iOutputName, const std::string& iInputName);
        virtual void clear();
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
        int findInput(const std::string& iName) const;
        int findOutput(const std::string& iName) const;


        void resize(int iWidth, int iHeight);
        bool isFboOwned() const { return mFboIsOwned; }

        void removeInput(int iIndex);
        void removeOutput(int iIndex);
        void setName(const std::string& iV) { mName = iV; }

    protected:
        virtual void initializeFbo() { ; }
        virtual void loadShader(const std::string& iAssetPath) { (void)iAssetPath; }
        virtual void sharePasses(const std::map<int, IRenderPass*> ipRenderPassNameToRenderPass) { ; }
        virtual void defineInputOutputs() = 0;
        virtual void connectInputOutputs() = 0;
        virtual void applyGlState() = 0;
        virtual void render(const Rendering::Camera&, const std::map<uint32_t, IRenderable*> ipRenderables) = 0;
        virtual void revertGlState() = 0;
        virtual void releaseGlRessources();

        static Input mDummyInput;
        static Output mDummyOutput;
        static Rendering::FrameBufferObject mDummyFbo;

        std::string mName;
        std::vector<Input> mInputs;
        std::vector<Output> mOutputs;
        std::vector<Connection> mConnections;

        Rendering::Shader mShader;
        Rendering::FrameBufferObject* mpFbo; // each pass decides if it owns of share an Fbo
        bool mFboIsOwned;
    };

}
}

