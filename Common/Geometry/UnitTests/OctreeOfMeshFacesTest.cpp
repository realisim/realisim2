
#include "Core/Unused.h"
#include "Core/FileInfo.h"
#include "Core/Path.h"

#include "gtest/gtest.h"

#include "3d/Loader/ObjLoader.h"
#include "Geometry/OctreeOfMeshFaces.h"
#include "Math/IsEqual.h"

using namespace Realisim;
    using namespace Core;
    using namespace Geometry;
    using namespace Math;
    using namespace ThreeD;

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

TEST(OctreeOfMeshFaces, constructor)
{

}

TEST(OctreeOfMeshFaces, functions)
{


}

TEST(OctreeOfMeshFaces, generate)
{
    OctreeOfMeshFaces octree;
    ObjLoader objLoader;

    std::string filePath = getAssetsPath() + "/cow.obj";
    ObjLoader::Asset asset = objLoader.load(filePath);
    octree.setMesh(asset.mMeshes[0]);
    octree.generate();

    printf("%s\n%s\n", filePath.c_str(), octree.statsToString().c_str());
}