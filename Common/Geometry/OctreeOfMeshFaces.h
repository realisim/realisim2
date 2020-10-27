
#pragma once

#include "AxisAlignedBoundingBox.h"
#include <Triangle.h>
#include <vector>
#include <string>

namespace Realisim
{
namespace Geometry
{
    class Mesh;

    class OctreeOfMeshFaces
    {
    public:
        OctreeOfMeshFaces();
        OctreeOfMeshFaces(const OctreeOfMeshFaces&) = delete;
        OctreeOfMeshFaces& operator=(const OctreeOfMeshFaces&) = delete;
        ~OctreeOfMeshFaces();

        struct Node
        {
            Node();
            ~Node();

            int getNumberOfChilds() const;
            bool hasChilds() const;

            //--- data
            static int sMaxNumberOfChilds;

            Node *mpParent;
            std::vector<Node*> mChilds;

            uint32_t mId;
            AxisAlignedBoundingBox mAabb;
            std::vector<uint32_t> mMeshFaceIndices;
            std::vector<Triangle> mTriangles; // only leafs have triangles
            
        };

        void clear();
        void generate();
        void generateFromMesh(Geometry::Mesh*);
        const Node* getRoot() const;
        const Mesh* getMesh() const;
        bool isGenerated() const;
        void setMesh(Geometry::Mesh*);
        std::string statsToString() const;

    protected:

        struct Stats
        {
            Stats() : mTotalNumberOfNodes(0), mOctreeDepth(0), mTimeToGenerateInSeconds(0.0) {}

            uint32_t mTotalNumberOfNodes;
            int32_t mOctreeDepth;
            double mTimeToGenerateInSeconds;
        };


        void assignPrism(Node *n, int iIndex);
        void assignFaceIndices(Node *n);
        void cleanupAndAssignLeafTriangles(Node *, int depthCount);
        void generate(Node *n, int iDepth);
        void validate(Node *n);

        Node *mpRoot; // never null, owned
        Mesh *mpMesh; //not owned
        Stats mStats;
        int mMaxNumberOfPolygonsPerNode;
    };
}
}