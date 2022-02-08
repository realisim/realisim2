
#pragma once

#include "3d/Scene/SceneNode.h"
#include "3d/Scene/IPositionableNode.h"
#include "DataStructures/Scene/MaterialNode.h"
#include "Geometry/Mesh.h"
#include <vector>


namespace Realisim
{
namespace Reactor
{
    class ModelNode : public ThreeD::SceneNode, public ThreeD::IPositionableNode {
    public:
        ModelNode();
        ModelNode(const ModelNode&) = delete;
        ModelNode& operator=(const ModelNode&) = delete;        
        virtual ~ModelNode();

        void addRegisteredRenderPass(int iRenderPassId);
        void clear();
        void addMesh(Geometry::Mesh& mesh);
        void addMesh(Geometry::Mesh* ipMesh);
        void addMeshes( const std::vector<Geometry::Mesh*> ipMeshes);
        int getNumberOfMeshes() const { return (int)mMeshPtrs.size(); }
        MaterialNode* getMaterialNode() { return mpMaterialNode; }
        const std::vector<Geometry::Mesh*>& getMeshes() const { return mMeshPtrs; }
        const std::set<int>& getRegisteredRenderPasses() const { return mRegisteredRenderPasses; }
        const Math::Vector2& getTextureScaling() const { return mTextureScaling; }
        virtual void initializeModelSpaceAABB() override;
        void removeRegisteredRenderPass(int iRenderPassId);
        void setRegisteredRenderPasses(const std::vector<int>&);
        void setMaterialNode(MaterialNode* ipNode) { mpMaterialNode = ipNode; }
        void setTextureScaling(double iX, double iY) { mTextureScaling.set(iX, iY); }
        virtual void update(double iTimeSinceStartupInSeconds) override;

    protected:
        std::vector<Geometry::Mesh*> mMeshPtrs; //owned
        MaterialNode* mpMaterialNode; // not owned
        Math::Vector2 mTextureScaling;

        std::set<int> mRegisteredRenderPasses;
    };

}
}

