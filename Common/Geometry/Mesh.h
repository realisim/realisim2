#pragma once

#include "Math/Vector.h"
#include <vector>

namespace Realisim
{
namespace Geometry
{
	/**/
    class Mesh
    {
    public:
		Mesh();
		Mesh(const Mesh&) = default;
		Mesh& operator=(const Mesh&) = default;
		~Mesh();

		struct Face
		{
			std::vector<int> mVertexIndices;
			Math::Vector3 mNormal;
		};

		void clear();
		std::vector<Face>& getFaces();
		const std::vector<Face>& getFaces() const;
		const Math::Vector3& getNormalOnFace(int iFaceIndex) const;
		int getNumberOfFaces() const;
		int getNumberOfVertices() const;
		int getNumberOfVerticesPerFace() const;
//int getNumberOfNormalsPerFace() const;
		std::vector<int>& getFaceIndices();
		const std::vector<int>& getFaceIndices() const;
		const Math::Vector3& getVertexOnFace(int iVertexIndex, int iFaceIndex) const;
		int getVertexIndexOnFace(int iVertexIndex, int iFaceIndex) const;
		const Math::Vector3& getVertex(int iIndex) const;
		std::vector<Math::Vector3>& getVertices();
		const std::vector<Math::Vector3>& getVertices() const;
		void setNumberOfVerticesPerFace(int iN);

    protected:
		int mNumberOfVerticesPerFace;
		std::vector<Math::Vector3> mVertices;
		std::vector<Face> mFaces;
		std::vector<int> mFaceIndices;
	};
}
}
