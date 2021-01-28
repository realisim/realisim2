
#include "Core/Timer.h"
#include "GeometryNodes.h"
#include "Geometry/AxisAlignedBoundingBox.h"
#include "Geometry/Intersections.h"
#include "IntersectionResult.h"

using namespace Realisim;
    using namespace Geometry;
    using namespace LightBeam;
    using namespace Math;

using namespace std;

//-------------------------------------------------------------------------
//--- Plane
//-------------------------------------------------------------------------
PlaneNode::PlaneNode() : ISceneNode(ntRenderable),
    IRenderable(),
    mPlane()
{
}

//-------------------------------------------------------------------------
const Plane& PlaneNode::getPlane() const
{ return mPlane; }

//-------------------------------------------------------------------------
bool PlaneNode::intersects(const Line& iRay) const
{
    return Geometry::intersects(iRay, mPlane);
}

//-------------------------------------------------------------------------
bool PlaneNode::intersect(const Line& iRay, IntersectionResult* opResult) const
{
    Vector3 p;
    Vector3 n;
    double d;
    Geometry::IntersectionType it = Geometry::intersect(iRay, mPlane, &p, &n, &d);
    
    if(opResult && it != itNone)
    {
        (*opResult).mNormal = n;
        (*opResult).mD = d;
        (*opResult).mpMaterialNode = getMaterialNode();
    }
    return it != itNone;
}

//-------------------------------------------------------------------------
void PlaneNode::setPlane(const Plane& iPlane)
{ mPlane = iPlane; }


//-------------------------------------------------------------------------
//--- Sphere
//-------------------------------------------------------------------------
SphereNode::SphereNode() : ISceneNode(ntRenderable),
    IRenderable(),
    mSphere()
{
}

//-------------------------------------------------------------------------
const Sphere& SphereNode::getSphere() const
{ return mSphere; }

//-------------------------------------------------------------------------
bool SphereNode::intersects(const Line& iRay) const
{ return Geometry::intersects(iRay, mSphere); }

//-------------------------------------------------------------------------
bool SphereNode::intersect(const Line& iRay, IntersectionResult* opResult) const
{
    std::vector<Vector3> p;
    std::vector<Vector3> n;
    std::vector<double> d;
    Geometry::IntersectionType it = Geometry::intersect(iRay, mSphere, &p, &n, &d);
    
    if(opResult && it == itPoints)
    {
        // We want the point closest to the origin of the
        // ray in the direction of the ray. This means the smallest d positive
        // d.
        //
        int index = 0;
        if(d[0] >=0 && d[1] >= 0)
        { index = d[0] < d[1] ? 0 : 1; }
        else if( d[0] < 0 )
        { index = 1; }
        else
        { index = 0 ;}
        
        (*opResult).mNormal = n[index];
        (*opResult).mD = d[index];
        (*opResult).mpMaterialNode = getMaterialNode();
    }
    return it != itNone;
}

//-------------------------------------------------------------------------
void SphereNode::setSphere(const Sphere& iS)
{ mSphere = iS; }


//-------------------------------------------------------------------------
//--- Mesh
//-------------------------------------------------------------------------
MeshNode::MeshNode() : ISceneNode(ntRenderable),
IRenderable(),
mpMesh(nullptr)
{
}

//-------------------------------------------------------------------------
MeshNode::~MeshNode()
{
    if (mpMesh)
    {
        delete mpMesh;
        mpMesh = nullptr;
    }
}

//-------------------------------------------------------------------------
const Mesh* MeshNode::getMesh() const
{
    return mpMesh;
}

//-------------------------------------------------------------------------
bool MeshNode::intersects(const Line& iRay) const
{
    return Geometry::intersects(iRay, getAxisAlignedBoundingBox());
}

//-------------------------------------------------------------------------
bool MeshNode::intersect(const Line& iRay, IntersectionResult* opResult) const
{
    //Core::Timer _t;

    IntersectionType iType = itNone;

    std::vector<Vector3> p;
    std::vector<Vector3> n;
    std::vector<double> d;
    std::vector<Vector2> uv;

    iType = Geometry::intersect(iRay, mOctree, &p, &n, &d, &uv);

    if (opResult && iType != itNone)
    {
        double v = std::numeric_limits<double>::max();
        int index = 0;
        for (int i = 0; i < d.size(); ++i)
        {
            if (d[i] > 0 && d[i] < v)
            {
                index = i;
                v = d[i];
            }
        }

        (*opResult).mNormal = n[index];
        (*opResult).mD = d[index];
        (*opResult).mpMaterialNode = getMaterialNode();
        (*opResult).mUV = uv[index];
    }

    
    //const AxisAlignedBoundingBox &aabb = getAxisAlignedBoundingBox();
    //if (Geometry::intersects(iRay, aabb))
    //{
    //    iType = Geometry::intersect(iRay, *mpMesh, &p, &n, &d);

    //    if (opResult && iType != itNone)
    //    {
    //        double v = std::numeric_limits<double>::max();
    //        int index = 0;
    //        for (int i = 0; i < d.size(); ++i)
    //        {
    //            if (d[i] > 0 && d[i] < v)
    //            {
    //                index = i;
    //                v = d[i];
    //            }
    //        }

    //        (*opResult).mNormal = n[index];
    //        (*opResult).mD = d[index];
    //        (*opResult).mpMaterial = getMaterial();
    //    }
    //}

    //printf("MeshNode::intersect elapsed (s): %.4f\n", _t.elapsed());

    return iType != itNone;
}

//-------------------------------------------------------------------------
void MeshNode::setMeshAndTakeOwnership(Mesh *ipMesh)
{
    if (mpMesh)
    {
        delete mpMesh;
        mpMesh = nullptr;
    }
    mpMesh = ipMesh;
    mOctree.generateFromMesh(ipMesh);

    printf("%s\n", mOctree.statsToString().c_str());

    // assign AABB
    setAxisAlignedBoundingBox(mOctree.getRoot()->mAabb);
}