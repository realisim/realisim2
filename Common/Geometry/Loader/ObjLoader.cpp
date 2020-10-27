
#include <cassert>
#include "Core/FileInfo.h"
#include "Core/Unused.h"
#include "ObjLoader.h"
#include <stdint.h>

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

typedef pair<uint32_t, uint32_t> TinyIndexPair;
//-----------------------------------------------------------------------------
// this function assumes the input is triangulated
void ObjLoader::createAsset(const tinyobj::attrib_t &iAttrib,
    const std::vector<tinyobj::shape_t>& iShapes,
    Asset *opAsset)
{
    // For each shape
    for (size_t i = 0; i < iShapes.size(); i++)
    {
        // this map is tricky...
        // Tiny obj has a vertex pool and a normal pool
        // We have no pool, we must have a vertex and normal pair (see Mesh::VertexData)
        // for that reason, we use tiny vertex index and normal index as the key of the map
        // to point to our unique VertexData index.
        //
        map<TinyIndexPair, uint32_t> mTinyIndexToMyIndex;

        Mesh *pMesh = new Mesh();
        pMesh->setNumberOfVerticesPerFace(3);

        // get the mesh guts, so we can fill the structures...
        vector<Mesh::VertexData>& vds = pMesh->getVerticesRef();
        vector<Mesh::Face>& faces = pMesh->getFacesRef();

        size_t index_offset = 0;
        bool generateNormal = true;
        // For each face
        // get the vertex, normal and texture uv
        for (size_t f = 0; f < iShapes[i].mesh.num_face_vertices.size(); f++) {
            size_t fnum = iShapes[i].mesh.num_face_vertices[f];

            assert(fnum == 3 && "Input must be triangulated...");

            // For each vertex in the face
            Mesh::Face face;
            face.mVertexIndices.resize(3);
            for (size_t v = 0; v < fnum; v++) {
                tinyobj::index_t idx = iShapes[i].mesh.indices[index_offset + v];

                const int tinyVIndex = idx.vertex_index;
                const int tinyNIndex = idx.normal_index;
                const int tinyTIndex = idx.texcoord_index;
                TinyIndexPair tinyIndexPair = make_pair(tinyVIndex, tinyNIndex);
                // get the vertex and add it to the mesh if necessary
                if (mTinyIndexToMyIndex.find(tinyIndexPair) == mTinyIndexToMyIndex.end())
                {
                    Mesh::VertexData vd;
                    vd.mVertex.set((double)iAttrib.vertices[3* tinyVIndex + 0],
                        (double)iAttrib.vertices[3* tinyVIndex + 1],
                        (double)iAttrib.vertices[3* tinyVIndex + 2]);
                    vds.push_back(vd);
                    mTinyIndexToMyIndex[tinyIndexPair] = (uint32_t)vds.size() - 1;
                }
                const int myIndex = mTinyIndexToMyIndex[tinyIndexPair];
                face.mVertexIndices[v] = myIndex;

                // has a normal
                if (tinyNIndex >= 0)
                {
                    vds[myIndex].mNormal.set(
                        (double)iAttrib.normals[3 * tinyNIndex + 0],
                        (double)iAttrib.normals[3 * tinyNIndex + 1],
                        (double)iAttrib.normals[3 * tinyNIndex + 2]);
                    generateNormal = false;
                }

                // has uv
                if (tinyTIndex >= 0)
                {
                    vds[myIndex].mLayerIndexToTextureCoordinates[0].set(
                        (double)iAttrib.texcoords[2 * tinyTIndex + 0],
                        (double)iAttrib.texcoords[2 * tinyTIndex + 1]);
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

        // add name and mesh to asset
        //
        opAsset->mName.push_back(iShapes[i].name);
        opAsset->mMeshes.push_back(pMesh);
    }

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
ObjLoader::Asset ObjLoader::load(const string& iFilePath)
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

    Asset asset;
    if (!hasErrors())
    {
        createAsset(attrib, shapes, &asset);
    }

    return asset;
}