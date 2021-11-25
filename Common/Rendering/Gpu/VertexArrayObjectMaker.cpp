#if defined( _MSC_VER )
#pragma warning(push, 0)
#pragma warning( disable : 4503)

// 4503 is disabled because of a vector<BufferObject*>
// it leads to very long names Simthetiq::Rendering::VertexNormalTexCoordData:...
// and it creates a warning wich we can ignore.
//
#endif

#include <cassert>
#include <limits>
#include "Math/Vector.h"
#include "Rendering/Gpu/VertexArrayObjectMaker.h"
#include "Rendering/Gpu/Utilities.h"

using namespace Realisim;
    using namespace Math;
using namespace std;

namespace Realisim
{
namespace Rendering
{

    //----------------------------------------------------------------------------
    VertexArrayObject* makeVao(const Geometry::Mesh& iMesh)
    {
        VboLayoutLocation layoutLocation;

        return makeVao(iMesh, layoutLocation);
    }

	//----------------------------------------------------------------------------
    VertexArrayObject* makeVao(const Geometry::Mesh& iMesh, const VboLayoutLocation& iLayoutLocation)
    {
        // create return pointer object
        VertexArrayObject *pVao = new VertexArrayObject();

        vector<BufferObject*> vbos = makeVertexBufferObjects(iMesh, iLayoutLocation);
        for (size_t i = 0; i < vbos.size(); ++i)
        {
            pVao->addAndTakeOwnership(vbos[i]);
        }

        Rendering::DrawMode dm = dmTriangles;
        assert(iMesh.getNumberOfVerticesPerFace() == 3);
        pVao->bake(dm);

        return pVao;
    }

 //   //----------------------------------------------------------------------------
 //   std::shared_ptr<VertexArrayObject> makeVaoPointCloud(const Geometry::Mesh& iMesh)
 //   {
 //       // create vbos
 //       BufferObject* vboVertex = new BufferObject();
 //       BufferObject* vobIndices = new BufferObject();

 //       const vector<Geometry::Mesh::VertexData>& vertexData = iMesh.getVertices();
 //       vector<Vector3> vertices(vertexData.size(), Vector3());
 //       for (int i = 0; i < vertexData.size(); ++i)
 //       { vertices[i] = vertexData[i].mVertex; }

 //       std::vector<float> floatVertex = toFloatArray(vertices);

 //       Rendering::BufferObjectData* vertexStructure = new Rendering::BufferObjectDataPlain();
 //       vertexStructure->addAttribute(DataType::dtFloat,3,BufferObject::lliVertex);

 //       vboVertex->setDataStructure(vertexStructure);

 //       vboVertex->assignData(bbtArrayBuffer,
 //           bduStaticDraw,
 //           iMesh.getVertices().size(),
 //           &floatVertex[0]
 //       );

	//	// create indices
	//	std::vector<unsigned int> indices(iMesh.getNumberOfVertices(), 0);
	//	for (int i = 0; i < iMesh.getNumberOfVertices(); ++i)
	//	{
	//		indices[i] = i;
	//	}

 //        Rendering::BufferObjectData* indiceStructure = new Rendering::BufferObjectDataPlain();
 //       indiceStructure->addAttribute(DataType::dtUnsignedInteger,1,0);

 //       vobIndices->setDataStructure(indiceStructure);
	//	vobIndices->assignData(bbtElementArrayBuffer,
	//		bduStaticDraw,
	//		indices.size(),
	//		&indices[0]
	//		);

	//	// create return pointer object
	//	shared_ptr<VertexArrayObject> pVao = make_shared<VertexArrayObject>();
	//	pVao->addAndTakeOwnership(vboVertex);
	//	pVao->addAndTakeOwnership(vobIndices);
	//	pVao->bake(dmPoints);
	//	return pVao;
	//}

    //----------------------------------------------------------------------------
    // IMPORTANT:
    //  These vbos are limited to 65536 vertices. If iMesh has more... undefined behavior
    //
    vector<BufferObject*> makeVertexBufferObjects(const Geometry::Mesh& iMesh,
        const VboLayoutLocation& iLayoutLocation)
    {
        vector<BufferObject*> vbosToReturn;

        struct TexCoordData
        {
            TexCoordData() : tu(0.f), tv(0.f) {}
            TexCoordData(const Vector2& iT) : tu((float)iT.x()), tv((float)iT.y()) {}
            float tu, tv;
        };

        // this structure is to pack vertex, normal and texture coordinate into a single
        // interleaved VBo.
        struct VertexNormalTexCoordData
        {
            VertexNormalTexCoordData() : vx(0), vy(0), vz(0), nx(0), ny(0), nz(0.f), t() {}
            VertexNormalTexCoordData(const Vector3& iV, const Vector3& iN, const Vector2& iT) :
                vx((float)iV.x()), vy((float)iV.y()), vz((float)iV.z()),
                nx((float)iN.x()), ny((float)iN.y()), nz((float)iN.z()),
                t(iT) {}
            float vx, vy, vz;
            float nx, ny, nz;
            TexCoordData t;
        };

        const int kAdditionnalTextureLayers = 5;

        // create the interleave data to be stored in the vbo
        // interleaved data
        const int nbVertices = iMesh.getNumberOfVertices();
        vector<VertexNormalTexCoordData> vntData(nbVertices, VertexNormalTexCoordData());
        vector<Vector3> tangents(nbVertices, Vector3());
        vector<Vector3> biTangents(nbVertices, Vector3());

        map<int, vector<TexCoordData>> additionalTextureCoordinates; //layer 1 to n...

        for (int i = 0; i < nbVertices; ++i)
        {

            const Geometry::Mesh::VertexData& vd = iMesh.getVertex(i);

            Vector2 t;
            if(iMesh.hasTextureCoordinateLayer(0))
            { t = vd.mLayerIndexToTextureCoordinates.at(0); }

            vntData[i] = VertexNormalTexCoordData(vd.mVertex, vd.mNormal, t);

            //--- tangent space vbos.
            if (iMesh.hasTangentSpaceData())
            {
                const Geometry::Mesh::TangentSpaceData& tsd = iMesh.getTangentSpaceData(i);

                tangents[i] = tsd.mTangent;
                biTangents[i] = tsd.mBiTangent;
            }

            // additional texture coordinates vbos... texture layer 1 to kAdditionnalTextureLayers
            for (int textureLayerIndex = 1; textureLayerIndex <= kAdditionnalTextureLayers; ++textureLayerIndex)
            {
                if (iMesh.hasTextureCoordinateLayer(textureLayerIndex))
                {
                    t = vd.mLayerIndexToTextureCoordinates.at(textureLayerIndex);
                    additionalTextureCoordinates[textureLayerIndex].push_back(TexCoordData(t));
                }
            }

        }

        // create return pointer object
        if(!vntData.empty() && 
            iLayoutLocation.mVertices != -1 &&
            iLayoutLocation.mNormals != -1 &&
            iLayoutLocation.mTextureCoordinates[0] != -1)
        {
            //enum layoutLocation {llVertex = 0, llNormal, llTexture, llTangent, llBiTangent, llCount};

            // create vbos
            // This can be replaced with interleave data as soon as we
            // have a VectorF.
            //
            BufferObject* vboVerticesNormalsTexCoord0 (nullptr);
            BufferObject* vboTangents(nullptr);
            BufferObject* vboBiTangents(nullptr);
            BufferObject* vobIndices(nullptr);
            map<int,BufferObject*> vboAddtionnalTexCoords;

            //--- vertices, normals and texCoords layer 0 are stored as interleaved data.
            Rendering::BufferObjectData* verticesNormalsTexCoord0Structure = new Rendering::BufferObjectDataInterleaved();
            verticesNormalsTexCoord0Structure->addAttribute(DataType::dtFloat, 3, iLayoutLocation.mVertices);
            verticesNormalsTexCoord0Structure->addAttribute(DataType::dtFloat, 3, iLayoutLocation.mNormals);
            verticesNormalsTexCoord0Structure->addAttribute(DataType::dtFloat, 2, iLayoutLocation.mTextureCoordinates[0]);

            vboVerticesNormalsTexCoord0 = new BufferObject();
            vboVerticesNormalsTexCoord0->setDataStructure(verticesNormalsTexCoord0Structure);
            vboVerticesNormalsTexCoord0->assignData(bbtArrayBuffer,bduStaticDraw,vntData.size(),&vntData[0]);

            // add to vbos
            vbosToReturn.push_back(vboVerticesNormalsTexCoord0);

            if(iMesh.hasTangentSpaceData() && iLayoutLocation.mTangent != -1 && iLayoutLocation.mBiTangent != -1)
            {
                //--- tangents
                std::vector<float> floatTangents = toFloatArray(tangents);
                Rendering::BufferObjectData* tangentStructure = new Rendering::BufferObjectDataPlain();
                tangentStructure->addAttribute(DataType::dtFloat,3,iLayoutLocation.mTangent);

                vboTangents = new BufferObject();
                vboTangents->setDataStructure(tangentStructure);
                vboTangents->assignData(bbtArrayBuffer,bduStaticDraw,tangents.size(),&floatTangents[0]);

                //--- biTangents
                std::vector<float> floatBiTangents = toFloatArray(biTangents);
                Rendering::BufferObjectData* biTangentStructure = new Rendering::BufferObjectDataPlain();
                biTangentStructure->addAttribute(DataType::dtFloat,3,iLayoutLocation.mBiTangent);

                vboBiTangents = new BufferObject();
                vboBiTangents->setDataStructure(biTangentStructure);
                vboBiTangents->assignData(bbtArrayBuffer,bduStaticDraw,biTangents.size(),&floatBiTangents[0]);

                // add to vbos
                vbosToReturn.push_back(vboTangents);
                vbosToReturn.push_back(vboBiTangents);
            }

            // additionnal texture coordinates vbos.
            // we are adding 5 texture layers to the vbos...
            //            
            for (int textureLayerIndex = 1; textureLayerIndex <= kAdditionnalTextureLayers; ++textureLayerIndex)
            {
                if (iMesh.hasTextureCoordinateLayer(textureLayerIndex) && iLayoutLocation.mTextureCoordinates[textureLayerIndex] != -1)
                {
                    Rendering::BufferObjectData* additionTexCoordStructure = new Rendering::BufferObjectDataPlain();
                    additionTexCoordStructure->addAttribute(DataType::dtFloat, 2, iLayoutLocation.mTextureCoordinates[textureLayerIndex]);

                    vboAddtionnalTexCoords[textureLayerIndex] = new BufferObject();
                    vboAddtionnalTexCoords[textureLayerIndex]->setDataStructure(additionTexCoordStructure);
                    vboAddtionnalTexCoords[textureLayerIndex]->assignData(bbtArrayBuffer, bduStaticDraw, 
                        additionalTextureCoordinates[textureLayerIndex].size(), 
                        &additionalTextureCoordinates[textureLayerIndex][0]);

                    // add to vbos
                    vbosToReturn.push_back(vboAddtionnalTexCoords[textureLayerIndex]);
                }
                else
                {
                    vboAddtionnalTexCoords[textureLayerIndex] = nullptr;
                }
            }

            //--- indices            
            const int numFaces = iMesh.getNumberOfFaces();
            const int numVerticesPerFace = iMesh.getNumberOfVerticesPerFace();
            const vector<Geometry::Mesh::Face>& faces = iMesh.getFaces();
            vector<uint16_t> indices(numFaces * numVerticesPerFace);

            for (int i = 0; i < numFaces; ++i)
            {
                for (int j = 0; j < numVerticesPerFace; ++j)
                {
                    indices[i * numVerticesPerFace + j] = (uint16_t)faces[i].mVertexIndices[j];
                }
            }

            Rendering::BufferObjectData* indiceStructure = new Rendering::BufferObjectDataPlain();
            assert(indices.size() > 0);
            assert(indices.size() <= numeric_limits<unsigned short>::max());
            indiceStructure->addAttribute(DataType::dtUnsignedShort, 1, 0);

            vobIndices = new BufferObject();
            vobIndices->setDataStructure(indiceStructure);
            vobIndices->assignData(bbtElementArrayBuffer,
                bduStaticDraw,
                (int)indices.size(),
                &indices[0]);

            vbosToReturn.push_back(vobIndices);
        }

        return vbosToReturn;
    }


    //----------------------------------------------------------------------------
    //--- VboLayoutLocation
    //----------------------------------------------------------------------------
    VboLayoutLocation::VboLayoutLocation() :
        mVertices(0),
        mNormals(1),
        mTextureCoordinates({2,-1,-1,-1,-1,-1,-1,-1}),
        mTangent(10),
        mBiTangent(11)
    {}

}
}

#if defined( _MSC_VER )
#pragma warning(pop)
#endif