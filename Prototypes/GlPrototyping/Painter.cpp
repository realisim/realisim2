
#include "Math/Vector.h"
#include "Painter.h"
#include "Rendering/Gpu/Utilities.h"

using namespace Realisim;
    using namespace Math;
    using namespace Rendering;
using namespace std;

//-----------------------------------------------------------------------------
Painter::Painter()
{}

//-----------------------------------------------------------------------------
Painter::~Painter()
{
}

//-----------------------------------------------------------------------------
void Painter::createAxis()
{
    const double l = 100;
    const Vector3 zero;
    const Vector3 x(l, 0, 0);
    const Vector3 y(0, l, 0);
    const Vector3 z(0, 0, l);

    const Vector4 r(1.0, 0.0, 0.0, 1.0);
    const Vector4 g(0.0, 1.0, 0.0, 1.0);
    const Vector4 b(0.0, 0.0, 1.0, 1.0);


    BufferObject *vboVertices, *vboColors, *vboIndices;
    vboVertices = new BufferObject();
    vboColors = new BufferObject();
    vboIndices = new BufferObject();

    Rendering::BufferObjectData* vertexStructure = new Rendering::BufferObjectDataPlain();
    vertexStructure->addAttribute(DataType::dtFloat, 3, BufferObject::lliVertex);

    Rendering::BufferObjectData* colorStructure = new Rendering::BufferObjectDataPlain();
    colorStructure->addAttribute(DataType::dtFloat, 4, BufferObject::lliColor);

    Rendering::BufferObjectData* indicesStructure = new Rendering::BufferObjectDataPlain();
    indicesStructure->addAttribute(DataType::dtUnsignedByte, 1, 0);

    vector<Vector3> verticesData = { zero, x, zero, y, zero, z };    
    std::vector<float> floatVertex = Rendering::toFloatArray(verticesData);
    vboVertices->setDataStructure(vertexStructure);
    vboVertices->assignData(bbtArrayBuffer, bduStaticDraw, verticesData.size(), &floatVertex[0]);

    vector<Vector4> colorData = { r, r, g, g, b, b };
    std::vector<float> floatColor = Rendering::toFloatArray(colorData);
    vboColors->setDataStructure(colorStructure);
    vboColors->assignData(bbtArrayBuffer, bduStaticDraw, colorData.size(), &floatColor[0]);

    // indices
    const int nNumbIndices = 6;
    unsigned char indices[nNumbIndices] = {
        0, 1,
        2, 3,
        4, 5
    };
    vboIndices->setDataStructure(indicesStructure);
    vboIndices->assignData(bbtElementArrayBuffer,
        bduStaticDraw,
        nNumbIndices,
        &indices[0]);

    mAxis.addAndTakeOwnership(vboVertices);
    mAxis.addAndTakeOwnership(vboColors);
    mAxis.addAndTakeOwnership(vboIndices);
    mAxis.bake(dmLines);
}

//-----------------------------------------------------------------------------
void Painter::createCube()
{
    const double s = 10.0 / 2;
    const Vector3 x(1, 0, 0);
    const Vector3 y(0, 1, 0);
    const Vector3 z(0, 0, 1);
   
    // this structure is to pack vertex, normal and texture coordinate into a single
        // interleaved VBo.
    struct VertexNormalDataStruct
    {
        VertexNormalDataStruct() : vx(0), vy(0), vz(0), nx(0), ny(0), nz(0.f) {}
        VertexNormalDataStruct(const Vector3& iV, const Vector3& iN) :
            vx((float)iV.x()), vy((float)iV.y()), vz((float)iV.z()),
            nx((float)iN.x()), ny((float)iN.y()), nz((float)iN.z()) {}
        float vx, vy, vz;
        float nx, ny, nz;
    };

    //--- define vertex and normals
    vector<VertexNormalDataStruct> vnData(24, VertexNormalDataStruct()); // 4 points x 6 faces
    //x face
    vnData[0] = VertexNormalDataStruct(Vector3(s, -s, -s), x);
    vnData[1] = VertexNormalDataStruct(Vector3(s, -s,  s), x);
    vnData[2] = VertexNormalDataStruct(Vector3(s,  s,  s), x);
    vnData[3] = VertexNormalDataStruct(Vector3(s,  s, -s), x);

    //-x face
    vnData[4] = VertexNormalDataStruct(Vector3(-s, -s, -s), -x);
    vnData[5] = VertexNormalDataStruct(Vector3(-s,  s, -s), -x);
    vnData[6] = VertexNormalDataStruct(Vector3(-s,  s, s), -x);
    vnData[7] = VertexNormalDataStruct(Vector3(-s, -s, s), -x);

    //y face
    vnData[8] =  VertexNormalDataStruct(Vector3(-s, s,  s), y);
    vnData[9] =  VertexNormalDataStruct(Vector3( s, s,  s), y);
    vnData[10] = VertexNormalDataStruct(Vector3( s, s, -s), y);
    vnData[11] = VertexNormalDataStruct(Vector3(-s, s, -s), y);

    //-y face
    vnData[12] = VertexNormalDataStruct(Vector3(-s, -s,  s), -y);
    vnData[13] = VertexNormalDataStruct(Vector3( s, -s,  s), -y);
    vnData[14] = VertexNormalDataStruct(Vector3( s, -s, -s), -y);
    vnData[15] = VertexNormalDataStruct(Vector3(-s, -s, -s), -y);

    //z face
    vnData[16] = VertexNormalDataStruct(Vector3(-s, -s, s), z);
    vnData[17] = VertexNormalDataStruct(Vector3(s, -s, s), z);
    vnData[18] = VertexNormalDataStruct(Vector3(s, s, s), z);
    vnData[19] = VertexNormalDataStruct(Vector3(-s, s, s), z);

    //z face
    vnData[20] = VertexNormalDataStruct(Vector3(-s, -s, -s), -z);
    vnData[21] = VertexNormalDataStruct(Vector3(s, -s, -s), -z);
    vnData[22] = VertexNormalDataStruct(Vector3(s, s, -s), -z);
    vnData[23] = VertexNormalDataStruct(Vector3(-s, s, -s), -z);

    //--- defines face indices
    vector<uint16_t> indices = {
        0, 3, 2,
        0, 2, 1,

        4, 7, 6,
        4, 6, 5,

        8, 9, 10,
        8, 10, 11,

        12, 15, 14,
        12, 14, 13,

        16, 17, 18,
        16, 18, 19,

        20, 23, 22,
        20, 22, 21
    };

    //--- define the buffer objects
    BufferObject* vboVerticesNormals, *vboColors, *vboIndices;
    vboVerticesNormals = new BufferObject();
    vboColors = new BufferObject();
    vboIndices = new BufferObject();

    //--- define the BufferObject internal data structure
    Rendering::BufferObjectData* vertexNormalStructure = new Rendering::BufferObjectDataInterleaved();
    vertexNormalStructure->addAttribute(DataType::dtFloat, 3, BufferObject::lliVertex);
    vertexNormalStructure->addAttribute(DataType::dtFloat, 3, BufferObject::lliNormal);

    Rendering::BufferObjectData* indicesStructure = new Rendering::BufferObjectDataPlain();
    indicesStructure->addAttribute(DataType::dtUnsignedShort, 1, 0);

    //--- Assign structures and data to BufferObjects
    vboVerticesNormals->setDataStructure(vertexNormalStructure);
    vboVerticesNormals->assignData(bbtArrayBuffer, bduStaticDraw, vnData.size(), &vnData[0]);

    vboIndices->setDataStructure(indicesStructure);
    vboIndices->assignData(bbtElementArrayBuffer, bduStaticDraw, indices.size(), &indices[0]);

    mCube.addAndTakeOwnership(vboVerticesNormals);
    mCube.addAndTakeOwnership(vboIndices);
    mCube.bake(dmTriangles);
}

//-----------------------------------------------------------------------------
void Painter::drawAxis()
{
    if (!mAxis.isBaked())
    { createAxis(); }

    mAxis.draw();
}

//-----------------------------------------------------------------------------
void Painter::drawCube()
{
    if (!mCube.isBaked())
    {
        createCube();
    }

    mCube.draw();
}