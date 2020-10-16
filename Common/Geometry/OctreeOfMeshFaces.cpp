
#include "Core/Timer.h"
#include "Geometry/AxisAlignedBoundingBox.h"
#include <cassert>
#include "Geometry/OctreeOfMeshFaces.h"
//#include "Geometry/LineSegment.h"
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
    mpRoot(new Node())
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
    // go over all faces and add to current node list if they intersect with the current node prism
    // to intersect:
    //  at least one vertices is contained
    //  if no vertices is contained, on of the edge intersects 

    AxisAlignedBoundingBox aabb;
    bool oneVertexIsContained = false;
    for (auto faceIndex : p->mFaceIndices)
    {
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

        //if (!oneVertexIsContained)
        //{
        //    //// try the edges
        //    //LineSegment e;
        //    //e.set(vds[f.mVertexIndices[0]].mVertex, vds[f.mVertexIndices[1]].mVertex);
        //    //if (intersects(e, n->mPrism))
        //    //{
        //    //    n->mFaceIndices.push_back(faceIndex);
        //    //    break;
        //    //}

        //    //e.set(vds[f.mVertexIndices[1]].mVertex, vds[f.mVertexIndices[2]].mVertex);
        //    //if (intersects(e, n->mPrism))
        //    //{
        //    //    n->mFaceIndices.push_back(faceIndex);
        //    //    break;
        //    //}

        //    //e.set(vds[f.mVertexIndices[2]].mVertex, vds[f.mVertexIndices[0]].mVertex);
        //    //if (intersects(e, n->mPrism))
        //    //{
        //    //    n->mFaceIndices.push_back(faceIndex);
        //    //    break;
        //    //}
        //}
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
void OctreeOfMeshFaces::cleanupAfterGenerate(Node *n)
{
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
        cleanupAfterGenerate(n->mChilds[i]);
    }
}

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

    //add all face indices to the first root
    const int numFaces = mpMesh->getNumberOfFaces();
    mpRoot->mFaceIndices.resize(numFaces);
    for (int i = 0; i < numFaces; ++i) {
        mpRoot->mFaceIndices[i] = i;
    }

    generate(mpRoot);

    // cleanup all nodes except leafs...
    cleanupAfterGenerate(mpRoot);

    mStats.mTimeToGenerateInSeconds = _t.elapsed();
}

//---------------------------------------------------------------------------------------------------------------------
void OctreeOfMeshFaces::generate(Node *n)
{
    // stop criterion...
    //
    // ~5% of total number of polygons.
    //
    if (n->mFaceIndices.size() <= 25)
    {
        return;
    }

    mStats.mOctreeDepth++;

    // split into childs, empty leaf childs will
    // not be kept
    n->mChilds.reserve(Node::sMaxNumberOfChilds);
    int numChilds = 0;
    for (int i = 0; i < Node::sMaxNumberOfChilds; ++i)
    {
        Node *c = new Node();
        c->mpParent = n;
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
    oss << "depth: " << mStats.mOctreeDepth << endl;
    oss << "total number of nodes: " << mStats.mTotalNumberOfNodes;

    return oss.str();
}



//---------------------------------------------------------------------------------------------------------------------
//--- OctreeOfMeshFaces::Node
//---------------------------------------------------------------------------------------------------------------------
int OctreeOfMeshFaces::Node::sMaxNumberOfChilds = 8;

OctreeOfMeshFaces::Node::Node() :
    mpParent(nullptr)
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