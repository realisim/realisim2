
#include "DataStructures/Scene/ModelNode.h"
#include "DataStructures/Scene/SceneNodeEnum.h"

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
{}

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