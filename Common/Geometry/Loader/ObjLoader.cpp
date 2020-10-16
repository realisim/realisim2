
#include <cassert>
#include "Core/FileInfo.h"
#include "Core/Unused.h"
#include "ObjLoader.h"

using namespace Realisim;
    using namespace Geometry;
using namespace std;

//-----------------------------------------------------------------------------
ObjLoader::ObjLoader()
{}

//-----------------------------------------------------------------------------
ObjLoader::~ObjLoader()
{}

//-----------------------------------------------------------------------------
void ObjLoader::addError(const std::string& iE) const
{
    mErrors += mErrors.empty() ? iE : "\n" + iE;
}

//-----------------------------------------------------------------------------
// this function assumes the input is triangulated
Mesh* ObjLoader::createMesh(const tinyobj::attrib_t &iAttrib,
    const std::vector<tinyobj::shape_t>& iShapes)
{
    Mesh *pMesh = new Mesh();

    pMesh->setNumberOfVerticesPerFace(3);
    vector<Mesh::VertexData>& vds = pMesh->getVerticesRef();
    vector<Mesh::Face>& faces = pMesh->getFacesRef();

    // copy all vertices
    for (size_t v = 0; v < iAttrib.vertices.size() / 3; v++) {
        Mesh::VertexData vd;
        vd.mVertex.set((double)iAttrib.vertices[3 * v + 0],
            (double)iAttrib.vertices[3 * v + 1],
            (double)iAttrib.vertices[3 * v + 2]);
        vds.push_back(vd);
    }

    assert(iShapes.size() == 1 && "Handle multiple shape not implemented...");

    // For each shape
    for (size_t i = 0; i < iShapes.size(); i++) {

        size_t index_offset = 0;
        bool generateNormal = true;
        // For each face
        // get the normal and texture uv
        for (size_t f = 0; f < iShapes[i].mesh.num_face_vertices.size(); f++) {
            size_t fnum = iShapes[i].mesh.num_face_vertices[f];

            assert(fnum == 3 && "Input must be triangulated...");

            // For each vertex in the face
            Mesh::Face face;
            face.mVertexIndices.resize(3);
            for (size_t v = 0; v < fnum; v++) {
                tinyobj::index_t idx = iShapes[i].mesh.indices[index_offset + v];

                const int vIndex = idx.vertex_index;
                face.mVertexIndices[v] = vIndex;
                // has a normal
                if (idx.normal_index >= 0)
                {
                    vds[vIndex].mNormal.set(
                        (double)iAttrib.normals[idx.normal_index + 0],
                        (double)iAttrib.normals[idx.normal_index + 1],
                        (double)iAttrib.normals[idx.normal_index + 2]);
                    generateNormal = false;
                }

                // has uv
                if (idx.texcoord_index >= 0)
                {
                    vds[vIndex].mLayerIndexToTextureCoordinates[0].set(
                        (double)iAttrib.texcoords[idx.texcoord_index + 0],
                        (double)iAttrib.texcoords[idx.texcoord_index + 1]);

                }
            }

            // add face to mesh
            if (generateNormal){
                pMesh->makeFace(face.mVertexIndices, false);
            }
            else {
                
                faces.push_back(face);
            }

            index_offset += fnum;
        }
    }

    return pMesh;
}
//-----------------------------------------------------------------------------
const string ObjLoader::getAndClearLastErrors() const
{
    string e = mErrors;
    mErrors = string();
    return e;
}

//-----------------------------------------------------------------------------
bool ObjLoader::hasErrors() const
{
    return !mErrors.empty();
}

//-----------------------------------------------------------------------------
Mesh* ObjLoader::load(const string& iFilePath)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warn;
    std::string err;

    Core::FileInfo fi;
    fi.setCached(false);
    fi.setFile(iFilePath);

    //always triangulate
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, fi.getFilePath().c_str(),
        fi.getPath().c_str(), true);

    if (!ret)
    {
        addError("Could not load/parse obj file: " + iFilePath);
        addError("tinyObj warning: " + warn);
        addError("tinyObj error: " + err);

        printf("ObjLoader::load error: %s\n", mErrors.c_str());
    }

    Mesh *pMesh = nullptr;
    if (!hasErrors())
    {
        pMesh = createMesh(attrib, shapes);
    }

    return pMesh;
}