
#include "Core/Logger.h"
#include "Systems/Renderer/RenderPasses/IRenderPass.h"

using namespace Realisim;
    using namespace Reactor;
    using namespace Rendering;
using namespace std;

IRenderPass::Input IRenderPass::mDummyInput;
IRenderPass::Output IRenderPass::mDummyOutput;
Rendering::FrameBufferObject IRenderPass::mDummyFbo(1, 1, 1);

//---------------------------------------------------------------------------------------------------------------------
IRenderPass::IRenderPass() :
    mpFbo(nullptr),
    mFboIsOwned(false)
{}

//---------------------------------------------------------------------------------------------------------------------
IRenderPass::~IRenderPass()
{
    clear();
}

//---------------------------------------------------------------------------------------------------------------------
void IRenderPass::clear()
{
    mInputs.clear();
    mOutputs.clear();
    releaseGlRessources();
}

//---------------------------------------------------------------------------------------------------------------------
void IRenderPass::addInput(const Input& iInput)
{
    mInputs.push_back(iInput);
}

//---------------------------------------------------------------------------------------------------------------------
void IRenderPass::addOutput(const Output& iOutput)
{
    mOutputs.push_back(iOutput);
}

//---------------------------------------------------------------------------------------------------------------------
void IRenderPass::connect(IRenderPass* ipParentPass, const std::string& iOutputName, const std::string& iInputName)
{
    const int outputIndex = ipParentPass->findOutput(iOutputName);;
    const int inputIndex = findInput(iInputName);
       

    if (outputIndex != -1 && inputIndex != -1) {
        Connection c(outputIndex, inputIndex);
        mConnections.push_back(c);
    } else {
        LOG_TRACE_ERROR(Core::Logger::llNormal, "Could not connect output [%s] to input [%s] in renderpass [%s].",
            iOutputName.c_str(), iInputName.c_str(), getName().c_str());
    }
}

//---------------------------------------------------------------------------------------------------------------------
int IRenderPass::findInput(const std::string& iName) const
{
    int index = -1;
    for (int i = 0; i < (int)mInputs.size(); ++i) {
        if (mInputs[i].mName == iName) {
            index = i;
            break;
        }
    }
    return index;
}

//---------------------------------------------------------------------------------------------------------------------
int IRenderPass::findOutput(const std::string& iName) const
{
    int index = -1;
    for (int i = 0; i < (int)mOutputs.size(); ++i) {
        if (mOutputs[i].mName == iName) {
            index = i;
            break;
        }
    }
    return index;
}

//---------------------------------------------------------------------------------------------------------------------
const IRenderPass::Input& IRenderPass::getInput(int iIndex) const
{
    return const_cast<IRenderPass*>(this)->getInputRef(iIndex);
}

//---------------------------------------------------------------------------------------------------------------------
IRenderPass::Input& IRenderPass::getInputRef(int iIndex)
{
    Input* r = &mDummyInput;
    if (iIndex >= 0 && iIndex < getNumberOfInputs())
    {
        r = &mInputs[iIndex];
    }
    return *r;
}

//---------------------------------------------------------------------------------------------------------------------
const FrameBufferObject& IRenderPass::getFboRef() const
{
    return const_cast<IRenderPass*>(this)->getFboRef();
}

//---------------------------------------------------------------------------------------------------------------------
FrameBufferObject& IRenderPass::getFboRef()
{
    FrameBufferObject* pFbo = &mDummyFbo;
    if (mpFbo != nullptr)
        pFbo = mpFbo;
    return *pFbo;
}

//---------------------------------------------------------------------------------------------------------------------
const IRenderPass::Output& IRenderPass::getOutput(int iIndex) const
{
    return const_cast<IRenderPass*>(this)->getOutputRef(iIndex);
}

//---------------------------------------------------------------------------------------------------------------------
IRenderPass::Output& IRenderPass::getOutputRef(int iIndex)
{
    Output* r = &mDummyOutput;
    if (iIndex >= 0 && iIndex < getNumberOfOutputs())
    {
        r = &mOutputs[iIndex];
    }
    return *r;
}

//---------------------------------------------------------------------------------------------------------------------
const IRenderPass::Output& IRenderPass::getOutput(const std::string& iName) const
{
    return const_cast<IRenderPass*>(this)->getOutputRef(iName);
}

//---------------------------------------------------------------------------------------------------------------------
IRenderPass::Output& IRenderPass::getOutputRef(const std::string& iName)
{
    const int index = findOutput(iName);

    if (index == -1) {
        LOG_TRACE_ERROR(Core::Logger::llNormal, "Could not find pass output [%s:%s].", getName().c_str(), iName.c_str());
    }

    return getOutputRef(index);
}

//---------------------------------------------------------------------------------------------------------------------
void IRenderPass::releaseGlRessources()
{
    mShader.clear();
    if (mFboIsOwned && mpFbo != nullptr) {
        delete mpFbo;
        mpFbo = nullptr;
        mFboIsOwned = false;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void IRenderPass::removeInput(int iIndex)
{
    if (iIndex >= 0 && iIndex < getNumberOfInputs())
    {
        mInputs.erase(mInputs.begin() + iIndex);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void IRenderPass::removeOutput(int iIndex)
{
    if (iIndex >= 0 && iIndex < getNumberOfOutputs())
    {
        mOutputs.erase(mOutputs.begin() + iIndex);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void IRenderPass::resize(int iWidth, int iHeight)
{
    if (mFboIsOwned && mpFbo)
    {
        mpFbo->resize(iWidth, iHeight);
    }
}