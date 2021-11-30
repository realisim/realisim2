
#include <cassert>
#include "DataStructures/Scene/ModelNode.h"
#include "Rendering/Gpu/VertexArrayObjectMaker.h"
#include "Systems/Renderer/ModelRenderable.h"

using namespace Realisim;
    using namespace Reactor;
    using namespace Rendering;
using namespace std;

//---------------------------------------------------------------------------------------------------------------------
ModelRenderable::ModelRenderable(ModelNode* ipNode) : IRenderable(ipNode),
    mpNode(ipNode)
{
    assert(mpNode != nullptr);
}

//---------------------------------------------------------------------------------------------------------------------
ModelRenderable::~ModelRenderable()
{
    releaseGpuRessources();
}

//---------------------------------------------------------------------------------------------------------------------
void ModelRenderable::initializeGpuRessources()
{
    releaseGpuRessources();

    const vector<Geometry::Mesh*>& meshes = mpNode->getMeshes();
    for (auto pMesh : meshes)
    {
        VertexArrayObject* pVao = makeVao(*pMesh);
        mVaoPtrs.push_back(pVao);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void ModelRenderable::draw()
{
    for (auto pVao : mVaoPtrs)
    {
        pVao->draw();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void ModelRenderable::releaseGpuRessources()
{
    for (auto pVao : mVaoPtrs)
    {
        pVao->clear();
    }
    mVaoPtrs.clear();
}