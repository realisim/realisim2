#include "Core/Unused.h"
#include "Core/FileInfo.h"
#include "Core/Path.h"

#include "gtest/gtest.h"
#include "Geometry/Intersections.h"
#include "Geometry/Loader/ObjLoader.h"
#include "Geometry/OctreeOfMeshFaces.h"

using namespace Realisim;
    using namespace Core;
    using namespace Geometry;
    using namespace Math;

namespace
{
    std::string getCurrentFolderPath()
    {
        Core::FileInfo fi(Path::getApplicationFilePath());
        return fi.getCanonicalPath();
    }

    std::string getAssetsPath()
    {
        // return path to executable
        Core::FileInfo fi(Path::getApplicationFilePath());
        return fi.getCanonicalPath() + "/../GeometryAssets";
    }
}

TEST(Intersections, linePlane)
{
    // 90 degrees
    {
        IntersectionType it0;
        Line l0( Vector3(0.0, 0.0, 0.0),
                Vector3(0.0, 1.0, 0.0));
        
        Plane p0(Vector3(0.0, 0.0, 0.0),
                 Vector3(0.0, 1.0, 0.0));
        
        EXPECT_TRUE(intersects(l0, p0));
        
        Vector3 r;
        it0 = intersect(l0, p0, &r);
        EXPECT_EQ(r, Vector3(0.0, 0.0, 0.0));
        EXPECT_EQ(it0, itPoint);
    }
    
    // parallel no contact
    {
        IntersectionType it0;
        Line l0( Vector3(0.0, 1.0, 0.0),
                Vector3(1.0, 1.0, 0.0));
        
        Plane p0(Vector3(0.0, 0.0, 0.0),
                 Vector3(0.0, 1.0, 0.0));
        
        EXPECT_FALSE(intersects(l0, p0));
        
        Vector3 r;
        it0 = intersect(l0, p0, &r);
        EXPECT_EQ(r, Vector3(0.0, 0.0, 0.0));
        EXPECT_EQ(it0, itNone);
    }
    
    // parallel with contact
    {
        IntersectionType it0;
        Line l0( Vector3(0.0, 0.0, 0.0),
                Vector3(1.0, 0.0, 0.0));
        
        Plane p0(Vector3(0.0, 0.0, 0.0),
                 Vector3(0.0, 1.0, 0.0));
        
        EXPECT_TRUE(intersects(l0, p0));
        
        Vector3 r;
        it0 = intersect(l0, p0, &r);
        EXPECT_EQ(r, Vector3(0.0, 0.0, 0.0));
        EXPECT_EQ(it0, itLine);
    }
    
}

TEST(Intersections, lineOctreeOfMeshFaces)
{
    OctreeOfMeshFaces octree;
    ObjLoader objLoader;

    std::string filePath = getAssetsPath() + "/cow.obj";
    std::vector<Mesh*> pMeshes = objLoader.load(filePath);
    octree.setMesh(pMeshes[0]);
    octree.generate();

    Line l;
    l.setOrigin( Vector3(0.0, 0.0, 100) );
    l.setDirection( Vector3(0.0, 0.0, -1) );

    std::vector<Vector3> ps, ns; // points and normals
    std::vector<double> ds; // distance from line origin.
    IntersectionType it = intersect(l, octree, &ps, &ns, &ds);
    UNUSED(it);
}