
#include <cassert>
#include "Mesh.h"

using namespace Realisim;
    using namespace Geometry;

//-----------------------------------------------------------------------------
Mesh::Mesh() :
	mNumberOfVerticesPerFace(0),
	mVertices(),
	mFaces(),
	mFaceIndices()
{

}

//-----------------------------------------------------------------------------
Mesh::~Mesh()
{
	clear();
}

//-----------------------------------------------------------------------------
void Mesh::clear()
{
	mNumberOfVerticesPerFace = 0;

	mVertices.clear();
	mFaces.clear();
	mFaceIndices.clear();
}

//-----------------------------------------------------------------------------
std::vector<Mesh::Face>& Mesh::getFaces()
{
	return mFaces;
}

//-----------------------------------------------------------------------------
const std::vector<Mesh::Face>& Mesh::getFaces() const
{
	return mFaces;
}

//-----------------------------------------------------------------------------
const Math::Vector3& Mesh::getNormalOnFace(int iFaceIndex) const
{
	assert(iFaceIndex < getNumberOfFaces());
	return mFaces.at(iFaceIndex).mNormal;
}

//-----------------------------------------------------------------------------
int Mesh::getNumberOfFaces() const
{
	return (int)mFaces.size();
}

//-----------------------------------------------------------------------------
int Mesh::getNumberOfVertices() const
{
	return (int)mVertices.size();
}

//-----------------------------------------------------------------------------
int Mesh::getNumberOfVerticesPerFace() const
{
	return mNumberOfVerticesPerFace;
}

//-----------------------------------------------------------------------------
// Returns vertex iVertexIndex on face iFaceIndex.
//
const Math::Vector3& Mesh::getVertexOnFace(int iVertexIndex, int iFaceIndex) const
{
	const int index = getVertexIndexOnFace(iVertexIndex, iFaceIndex);
	return mVertices[index];
}

//-----------------------------------------------------------------------------
// Returns the vertex index for vertex iVertexIndex on face iFaceIndex.
// The vertex index returned can be used directly with mVertices.
//
int Mesh::getVertexIndexOnFace(int iVertexIndex, int iFaceIndex) const
{
	assert(iVertexIndex < getNumberOfVerticesPerFace());
	assert(iFaceIndex < getNumberOfFaces());

	const Face& f = mFaces.at(iFaceIndex);
	return f.mVertexIndices.at(iVertexIndex);
}

//-----------------------------------------------------------------------------
const Math::Vector3& Mesh::getVertex(int iIndex) const
{
	return mVertices[iIndex];
}

//-----------------------------------------------------------------------------
std::vector<Math::Vector3>& Mesh::getVertices()
{
	return mVertices;
}

//-----------------------------------------------------------------------------
const std::vector<Math::Vector3>& Mesh::getVertices() const
{
	return mVertices;
}

//-----------------------------------------------------------------------------
std::vector<int>& Mesh::getFaceIndices()
{
	return mFaceIndices;
}


//-----------------------------------------------------------------------------
const std::vector<int>& Mesh::getFaceIndices() const
{
	return mFaceIndices;
}

//-----------------------------------------------------------------------------
void Mesh::setNumberOfVerticesPerFace(int iN)
{
	mNumberOfVerticesPerFace = iN;
}