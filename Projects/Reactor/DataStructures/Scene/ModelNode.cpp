
#include "DataStructures/Scene/ModelNode.h"
#include "DataStructures/Scene/SceneNodeEnum.h"
#include "Systems/Renderer/RenderPasses/RenderPassId.h"

using namespace Realisim;
using namespace Geometry;
using namespace Reactor;
using namespace std;

//---------------------------------------------------------------------------------------------------------------------
ModelNode::ModelNode() : SceneNode((int)SceneNodeEnum::sneModelNode),
    IPositionableNode(),
    mMeshPtrs(),
    mpMaterialNode(nullptr),
    mTextureScaling(1, 1)
{
    mRegisteredRenderPasses = { rpiOpaque };
}

//---------------------------------------------------------------------------------------------------------------------
ModelNode::~ModelNode()
{
    clear();
}

//---------------------------------------------------------------------------------------------------------------------
// This method is rather inefficient as it will copy the mesh data
//
void ModelNode::addMesh(Mesh& mesh)
{
    Mesh* pMesh = new Mesh(mesh);
    mMeshPtrs.push_back(pMesh);
}

//---------------------------------------------------------------------------------------------------------------------
//
void ModelNode::addMesh(Mesh* ipMesh)
{
    mMeshPtrs.push_back(ipMesh);
}

//---------------------------------------------------------------------------------------------------------------------
void ModelNode::addMeshes(const std::vector<Mesh*> ipMeshes)
{
    for (auto pMesh : ipMeshes) {
        mMeshPtrs.push_back(pMesh);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void ModelNode::addRegisteredRenderPass(int iRenderPassId)
{
    mRegisteredRenderPasses.insert(iRenderPassId);
}

//---------------------------------------------------------------------------------------------------------------------
void ModelNode::clear()
{
    for (auto pMesh : mMeshPtrs)
    {
        delete pMesh;
    }
    mMeshPtrs.clear();
}

//---------------------------------------------------------------------------------------------------------------------
void ModelNode::initializeModelSpaceAABB()
{
    mOriginalModelSpaceAABB.reset();

    for (const auto pMesh : mMeshPtrs) {
        const uint32_t numVertices = pMesh->getNumberOfVertices();
        const vector<Mesh::VertexData>& vertices = pMesh->getVertices();
        for (const auto& v : vertices) {
            mOriginalModelSpaceAABB.addPoint(v.mVertex);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void ModelNode::removeRegisteredRenderPass(int iRenderPassId)
{
    mRegisteredRenderPasses.erase(iRenderPassId);
}

//---------------------------------------------------------------------------------------------------------------------
// sets which passes will render this model Node.
// It is to note, that once the model has been added to the renderer, it is not possible to change the render passes
// via this function.
//
// In order to change render passes once the objects have been passed to the renderer, you must call methods on the
// renderer to do so. If the renderer modifies the registeredRenderPasses, the modification will also affect the ModelNode.
//
void ModelNode::setRegisteredRenderPasses(const std::vector<int>& iV) {
    for (const auto iId : iV)
        addRegisteredRenderPass(iId);
}

//---------------------------------------------------------------------------------------------------------------------
void ModelNode::update(double iTimeSinceStartupInSeconds)
{
    (void)iTimeSinceStartupInSeconds;

    if (isTransformDirty()) {
        Math::Matrix4 parentWorldTransform;
        IPositionableNode* parent = findFirstParentOfType<IPositionableNode>();
        if (parent != nullptr)
        {
            parentWorldTransform = parent->getWorldTransform();
        }
        mWorldTransform = parentWorldTransform * mParentTransform;
        setTransformDirty(false);
    }
}