#pragma once

#include <array>
#include "Geometry/Mesh.h"
#include <memory>
#include "Rendering/Gpu/VertexArrayObject.h"

namespace Realisim
{
namespace Rendering
{

    // This structure is used to describe the vbo to shader binding locations
    // a location of -1 will mean that the vbo can be ignored. In case of -1,
    // the vbo is not created nor binded.
    //
    struct VboLayoutLocation
    {
        VboLayoutLocation();

        int mVertices;
        int mNormals;
        std::array<int, 8> mTextureCoordinates;
        int mTangent;
        int mBiTangent;
    };

    // Il me semble qu'on devrait mettre ces methodes dans la class Context... (qui je n'ai pas dans cette branche...)
    //
    VertexArrayObject* makeVao(const Geometry::Mesh&);
    VertexArrayObject* makeVao(const Geometry::Mesh&, const VboLayoutLocation& iLayoutLocation);
	//VertexArrayObject* makeVaoPointCloud(const Geometry::Mesh&);

    std::vector<BufferObject*> makeVertexBufferObjects(const Geometry::Mesh&, const VboLayoutLocation& iLayoutLocation);
}
}