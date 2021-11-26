#pragma once

#include <array>
#include "Math/Vector.h"
#include <map>
#include <unordered_set>
#include <vector>

namespace Realisim
{
namespace Geometry
{
	/*
        Note:
            Mesh is limited to 65536 vertexData.      
    */
    class Mesh
    {
    public:
		Mesh();
		Mesh(const Mesh&) = default;
		Mesh& operator=(const Mesh&) = default;
		~Mesh();

        // c'est ûn peu de garbarge...
        // on devrait avoir un 
        //  pool de vertex
        //  pool de normal
        //  pool de texture coords
        //
        // ensuite le vertex data ne fait qu'indexer dans le pool...
        // en ce moment, on duplique énormément de data...
        //

        struct VertexData
        {
            VertexData() = default;
            VertexData(const VertexData&) = default;
            VertexData& operator=(const VertexData&) = default;
            ~VertexData() = default;

            Math::Vector3 mVertex;
            Math::Vector3 mNormal;
            std::map<int, Math::Vector2 > mLayerIndexToTextureCoordinates;
            //Color?
        };

        struct TangentSpaceData
        {
            TangentSpaceData() = default;
            TangentSpaceData(const TangentSpaceData&) = default;
            TangentSpaceData& operator=(const TangentSpaceData&) = default;
            ~TangentSpaceData() = default;

            Math::Vector3 mTangent;
            Math::Vector3 mBiTangent;
        };

		struct Face
		{
            std::vector<uint32_t> mVertexIndices;
		};

		void clear();
        void computeTangentBasis();
        static void cutIntoSmallerMeshes(const Mesh& iMesh, const uint32_t iMaxNumberOfIndices, std::vector<Mesh> *opMeshes);
        
        void generateFlatNormals();
        //void generateSmoothNormals();

        Math::Vector3 getCenterPositionOfFace(int iFaceIndex) const;
		std::vector<Face>& getFacesRef();
		const std::vector<Face>& getFaces() const;
        int getFaceIndexUsingVertexIndex(uint32_t iVertexIndex) const;

		int getNumberOfFaces() const;
        int getNumberOfTextureCoordinateLayers() const;
		int getNumberOfVertices() const;
		int getNumberOfVerticesPerFace() const;
        
        const TangentSpaceData& getTangentSpaceData(int iVertexIndex) const;
        const std::vector<TangentSpaceData>& getTangentSpaceData() const;
        std::vector<TangentSpaceData>& getTangentSpaceDataRef();    
        const TangentSpaceData& getTangentSpaceDataOnFace(int iVertexIndex, int iFaceIndex) const;
        std::vector<int> getTextureCoordinateLayerIndices() const;

		const VertexData& getVertex(int iIndex) const;
        const VertexData& getVertexOnFace(int iVertexIndex, int iFaceIndex) const;		
		const std::vector<VertexData>& getVertices() const;
        std::vector<VertexData>& getVerticesRef();

        bool hasTangentSpaceData() const;
        bool hasTextureCoordinateLayers() const;
        bool hasTextureCoordinateLayer(int iLayerIndex) const;

        int makeFace(const std::vector<uint32_t>& iVertexIndices);
		void setNumberOfVerticesPerFace(int iN);
        void triangulate();

    protected:
        int getVertexIndexOnFace(int iVertexIndex, int iFaceIndex) const;

		int mNumberOfVerticesPerFace;
        std::vector<VertexData> mVertexData;
        std::vector<TangentSpaceData> mTangentSpaceData;

		std::vector<Face> mFaces;

        static VertexData mDummyVertexData;
        static TangentSpaceData mDummyTangentSpaceData;
	};
}
}
