
#include "Core/Timer.h"
#include "Geometry/AxisAlignedBoundingBox.h"
#include <cassert>
#include "Geometry/Intersections.h"
#include "Geometry/OctreeOfMeshFaces.h"
#include "Math/Vector.h"
#include <sstream>

using namespace Realisim;
    using namespace Geometry;
    using namespace Math;
using namespace std;

//---------------------------------------------------------------------------------------------------------------------
//--- OctreeOfMeshFaces
//---------------------------------------------------------------------------------------------------------------------
OctreeOfMeshFaces::OctreeOfMeshFaces() :
    mpRoot(new Node()),
    mMaxNumberOfPolygonsPerNode(25)
{}

//---------------------------------------------------------------------------------------------------------------------
OctreeOfMeshFaces::~OctreeOfMeshFaces()
{}

//---------------------------------------------------------------------------------------------------------------------
// index refers to a split as follow: 
//
// front view XY
// near layer (z positive)
//
// Y
// ^
// ---------
// | 3 | 2 |
// ---------
// | 0 | 1 |
// ---------> X
//
// far layer (half depth offset)
//
// Y
// ^
// ---------
// | 7 | 6 |
// ---------
// | 4 | 5 |
// ---------> X
//
void OctreeOfMeshFaces::assignPrism(Node *n, int iIndex)
{
    Node *p = n->mpParent;
    if (!p) return;

    Vector3 bl, tr; //bottom left, top right
    Vector3 s = p->mAabb.getSize();
    Vector3 half(s.x() * 0.5,
        s.y() * 0.5,
        s.z() * 0.5);

    switch (iIndex)
    {
    case 0: 
        bl = p->mAabb.getMinCorner();
        tr = bl + half;
        break;
    case 1:
        bl = p->mAabb.getMinCorner();
        bl.setX(bl.x() + half.x());

        tr = bl + half;
        break;
    case 2:
        bl = p->mAabb.getMinCorner();
        bl.setX(bl.x() + half.x());
        bl.setY(bl.y() + half.y());

        tr = bl + half;
        break;
    case 3:
        bl = p->mAabb.getMinCorner();
        bl.setY(bl.y() + half.y());

        tr = bl + half;
        break;
    case 4:
        bl = p->mAabb.getMinCorner();
        bl.setZ(bl.z() + half.z());

        tr = bl + half;
        break;
    case 5:
        bl = p->mAabb.getMinCorner();
        bl.setX(bl.x() + half.x());
        bl.setZ(bl.z() + half.z());

        tr = bl + half;
        break;
    case 6:
        bl = p->mAabb.getMinCorner();
        bl.setX(bl.x() + half.x());
        bl.setY(bl.y() + half.y());
        bl.setZ(bl.z() + half.z());

        tr = bl + half;
        break;
    case 7:
        bl = p->mAabb.getMinCorner();
        bl.setY(bl.y() + half.y());
        bl.setZ(bl.z() + half.z());

        tr = bl + half;
        break;
    default: assert(false); break;
    }

    n->mAabb.set(bl, tr);
}

//---------------------------------------------------------------------------------------------------------------------
void OctreeOfMeshFaces::assignFaceIndices(Node *n)
{
    Node *p = n->mpParent;
    if (!p) return;

    const vector<Mesh::VertexData> &vds = mpMesh->getVertices();
    const vector<Mesh::Face> &faces = mpMesh->getFaces();
    // go over all faces and add to current node list if they intersect with the current node aabb.
    // to intersect:
    //  at least one vertices is contained
    //  if no vertices is contained, check the triangle plane intersection with
    //  the current node aabb
    bool oneVertexIsContained = false;
    for (auto faceIndex : p->mFaceIndices)
    {
        oneVertexIsContained = false;

        const Mesh::Face &f = faces[faceIndex];
        for (auto vertexIndex : f.mVertexIndices)
        {
            if (n->mAabb.contains(vds[vertexIndex].mVertex))
            {
                n->mFaceIndices.push_back(faceIndex);
                oneVertexIsContained = true;
                break;
            }
        }

        if (!oneVertexIsContained)
        {
            Triangle tri;
            tri.set(
                vds[f.mVertexIndices[0]].mVertex,
                vds[f.mVertexIndices[1]].mVertex,
                vds[f.mVertexIndices[2]].mVertex);
            
            if (intersects(tri, n->mAabb))
            {
                n->mFaceIndices.push_back(faceIndex);
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void OctreeOfMeshFaces::clear()
{
    delete mpRoot;
    mpRoot = new Node();

    mStats = Stats();
}

//---------------------------------------------------------------------------------------------------------------------
void OctreeOfMeshFaces::cleanupAndAssignLeafTriangles(Node *n, int depthCount)
{
    mStats.mOctreeDepth = max(mStats.mOctreeDepth, depthCount);

    if (n->hasChilds())
    {
        n->mFaceIndices.clear();
    }
    else
    {
        Triangle tri;
        const int numFaces = (int)n->mFaceIndices.size();
        n->mTriangles.resize(numFaces);

        // fill the triangle list since this is a leaf
        //
        const std::vector<Mesh::Face> &faces = mpMesh->getFaces();
        const std::vector<Mesh::VertexData> &vertices = mpMesh->getVertices();
        for (int i = 0; i < numFaces; ++i)
        {
            const int faceIndex = n->mFaceIndices[i];
            tri.set(vertices[faces[faceIndex].mVertexIndices[0]].mVertex,
                vertices[faces[faceIndex].mVertexIndices[1]].mVertex,
                vertices[faces[faceIndex].mVertexIndices[2]].mVertex);

            n->mTriangles[i] = tri;
        }

        n->mFaceIndices.clear();
    }

    const int numC = n->getNumberOfChilds();
    for (int i = 0; i < numC; ++i) {
        cleanupAndAssignLeafTriangles(n->mChilds[i], depthCount++);
    }
}

#define VALIDATE_GENERATION
//---------------------------------------------------------------------------------------------------------------------
void OctreeOfMeshFaces::generate()
{
    Core::Timer _t;

    //early out
    if (!mpMesh) return;

    // create AABB to generate first node.
    const uint32_t numVertices = mpMesh->getNumberOfVertices();
    const vector<Mesh::VertexData> vds = mpMesh->getVertices();

    for (uint32_t i = 0; i < numVertices; ++i)
    {
        mpRoot->mAabb.addPoint(vds[i].mVertex);
    }

    mStats.mTotalNumberOfNodes = 1;

    //add all face indices to the first root
    const int numFaces = mpMesh->getNumberOfFaces();
    mpRoot->mFaceIndices.resize(numFaces);
    for (int i = 0; i < numFaces; ++i) {
        mpRoot->mFaceIndices[i] = i;
    }

    generate(mpRoot);

#ifdef VALIDATE_GENERATION
    validate(mpRoot);
#endif
    // cleanup all nodes except leafs...
    int depthCount = 1;
    cleanupAndAssignLeafTriangles(mpRoot, depthCount);

    mStats.mTimeToGenerateInSeconds = _t.elapsed();
}

//---------------------------------------------------------------------------------------------------------------------
void OctreeOfMeshFaces::generate(Node *n)
{
    // root has id 0
    static uint32_t idCounter = 1;
    // stop criterion...
    //
    // ~5% of total number of polygons.
    //
    if (n->mFaceIndices.size() <= mMaxNumberOfPolygonsPerNode)
    {
        return;
    }


    // do gepth first
    // !!!!! NOTE !!!!
    // This is actually not the best of idea as it might bust the stack...
    // going breadth first would be a lot better!
    //
    // split into childs, empty leaf childs will not be kept
    n->mChilds.reserve(Node::sMaxNumberOfChilds);
    int numChilds = 0;
    for (int i = 0; i < Node::sMaxNumberOfChilds; ++i)
    {
        Node *c = new Node();
        c->mpParent = n;
        c->mId = idCounter++;
        assignPrism(c, i);
        assignFaceIndices(c);

        if (!c->mFaceIndices.empty())
        {
            n->mChilds.push_back(c);
            numChilds++;
            generate(c);
        }
        else
        {
            delete c;
        }
    }
    n->mChilds.resize(numChilds);
    mStats.mTotalNumberOfNodes += numChilds;
}

//---------------------------------------------------------------------------------------------------------------------
void OctreeOfMeshFaces::generateFromMesh(Geometry::Mesh* ipMesh)
{
    setMesh(ipMesh);
    generate();
}

//---------------------------------------------------------------------------------------------------------------------
const OctreeOfMeshFaces::Node* OctreeOfMeshFaces::getRoot() const
{
    return mpRoot;
}

//---------------------------------------------------------------------------------------------------------------------
const Mesh* OctreeOfMeshFaces::getMesh() const
{
    return mpMesh;
}

//---------------------------------------------------------------------------------------------------------------------
bool OctreeOfMeshFaces::isGenerated() const
{
    return mpRoot->hasChilds() || !mpRoot->mFaceIndices.empty();
}

//---------------------------------------------------------------------------------------------------------------------
void OctreeOfMeshFaces::setMesh(Geometry::Mesh* ipMesh)
{
    if (mpMesh != ipMesh)
    {
        if (isGenerated()) {
            clear();
        }
    }
    mpMesh = ipMesh;
}

//---------------------------------------------------------------------------------------------------------------------
std::string OctreeOfMeshFaces::statsToString() const
{
    ostringstream oss;
    oss.precision(4);
    oss << fixed;
    oss << "---OctreeOfMeshFaces Stats---" << endl;
    oss << "time to generate (s): " << mStats.mTimeToGenerateInSeconds << endl;
    oss << "number of polygons in original mesh: " << (mpMesh ? mpMesh->getNumberOfFaces() : 0) << endl;
    oss << "max. number of polygons per node: " << mMaxNumberOfPolygonsPerNode  << endl;
    oss << "depth: " << mStats.mOctreeDepth << endl;
    oss << "total number of nodes: " << mStats.mTotalNumberOfNodes;

    return oss.str();
}

//---------------------------------------------------------------------------------------------------------------------
void OctreeOfMeshFaces::validate(Node *n)
{
    // validate that all face in parent have been accounted for in childs...

    map<int, bool> mParentFaceIdToBool;
    const int numFacesInParent = (int)n->mFaceIndices.size();
    for (auto &fi : n->mFaceIndices)
    {
        mParentFaceIdToBool[fi] = false;
    }

    // go over childs
    if (n->hasChilds())
    {
        for (auto c : n->mChilds)
        {
            for (auto &fi : c->mFaceIndices)
            {
                mParentFaceIdToBool[fi] = true;
            }
        }

        for (auto &it : mParentFaceIdToBool)
        {
            if (it.second == false)
            {
                printf("face %d is missing in childs from parent %d\n", it.first, n->mId);
            }
        }
    }

    const int numC = n->getNumberOfChilds();
    for (int i = 0; i < numC; ++i) {
        validate(n->mChilds[i]);
    }
}

//---------------------------------------------------------------------------------------------------------------------
//--- OctreeOfMeshFaces::Node
//---------------------------------------------------------------------------------------------------------------------
int OctreeOfMeshFaces::Node::sMaxNumberOfChilds = 8;

OctreeOfMeshFaces::Node::Node() :
    mpParent(nullptr),
    mId(0)
{}

//---------------------------------------------------------------------------------------------------------------------
OctreeOfMeshFaces::Node::~Node()
{
    mpParent = nullptr;
    // delete all childs.
    if (hasChilds())
    {
        const int n = getNumberOfChilds();
        for (int i = 0; i < n; ++i)
        {
            delete mChilds[i];
            mChilds[i] = nullptr;
        }
        mChilds.clear();
    }
    
}

//---------------------------------------------------------------------------------------------------------------------
int OctreeOfMeshFaces::Node::getNumberOfChilds() const
{
    return (int)mChilds.size();
}

//---------------------------------------------------------------------------------------------------------------------
bool OctreeOfMeshFaces::Node::hasChilds() const
{
    return !mChilds.empty();
}