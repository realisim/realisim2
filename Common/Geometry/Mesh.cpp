
#include <cassert>
#include "Mesh.h"

using namespace Realisim;
    using namespace Geometry;
    using namespace Math;
using namespace std;

#pragma message("--- Documenter le header et les fonctions et faire une examples d'utilisation  ---")

Mesh::VertexData Mesh::mDummyVertexData;
Mesh::TangentSpaceData Mesh::mDummyTangentSpaceData;

//-----------------------------------------------------------------------------
Mesh::Mesh() :
	mNumberOfVerticesPerFace(3),
    mVertexData(),
    mTangentSpaceData(),
	mFaces()
{}

//-----------------------------------------------------------------------------
Mesh::~Mesh()
{
	clear();
}

//-----------------------------------------------------------------------------
void Mesh::clear()
{
	//mNumberOfVerticesPerFace = 3; intentionnally not clearing the number of vertices
    // per face.. we only clear the data.

    mVertexData.clear();
    mTangentSpaceData.clear();
    mFaces.clear();
}

//-----------------------------------------------------------------------------
// Cette méthode remplie le vecteur mTangents et mBiTangents qui sont
// nécessaire pour créer la base orthogonal de l'espace tangent utilisé lors
// du normal mapping.
//
// Calcul de l'espace tangent:
// http://www.opengl-tutorial.org/fr/intermediate-tutorials/tutorial-13-normal-mapping/
//
void Mesh::computeTangentBasis()
{
    mTangentSpaceData.clear();

    // early out
    // This works only for triangulated faces...
    //
    assert(mNumberOfVerticesPerFace == 3);
    if(mNumberOfVerticesPerFace != 3)
    { return; }

    mTangentSpaceData.resize(mVertexData.size());    
    mTangentSpaceData.shrink_to_fit();

    Vector3 deltaPos1, deltaPos2;
    Vector2 deltaUv1, deltaUv2;
    Vector3 tangent, biTangent;
    const int numFaces = (int)mFaces.size();    
    for (int i = 0; i < numFaces; ++i)
    {
        const VertexData& vd0 = getVertexOnFace(0, i);
        const VertexData& vd1 = getVertexOnFace(1, i);
        const VertexData& vd2 = getVertexOnFace(2, i);

        const Vector3& v0 = vd0.mVertex;
        const Vector3& v1 = vd1.mVertex;
        const Vector3& v2 = vd2.mVertex;

        const Vector2& uv0 = vd0.mLayerIndexToTextureCoordinates.at(0);
        const Vector2& uv1 = vd1.mLayerIndexToTextureCoordinates.at(0);
        const Vector2& uv2 = vd2.mLayerIndexToTextureCoordinates.at(0);

        deltaPos1 = v1 - v0;
        deltaPos2 = v2 - v0;

        deltaUv1 = uv1 - uv0;
        deltaUv2 = uv2 - uv0;

        const double r = 1.0 / (deltaUv1.x() * deltaUv2.y() - deltaUv1.y() * deltaUv2.x());
        tangent = (deltaPos1 * deltaUv2.y()   - deltaPos2 * deltaUv1.y())*r;
        biTangent = (deltaPos2 * deltaUv1.x()   - deltaPos1 * deltaUv2.x())*r;

        // make sure our space is orthogonal
        //
        const Vector3& n = vd0.mNormal;
        tangent = (tangent - (n * (n * tangent))).normalize();

        if ((n ^ tangent) * biTangent < 0) // normal is inverted...
        {
            tangent *= -1;
        }
       
        mTangentSpaceData[ getVertexIndexOnFace(0, i) ].mTangent = tangent;
        mTangentSpaceData[ getVertexIndexOnFace(1, i) ].mTangent = tangent;
        mTangentSpaceData[ getVertexIndexOnFace(2, i) ].mTangent = tangent;

        mTangentSpaceData[ getVertexIndexOnFace(0, i) ].mBiTangent = biTangent;
        mTangentSpaceData[ getVertexIndexOnFace(1, i) ].mBiTangent = biTangent;
        mTangentSpaceData[ getVertexIndexOnFace(2, i) ].mBiTangent = biTangent;
    }
    
}

//-----------------------------------------------------------------------------
void Mesh::cutIntoSmallerMeshes(const Mesh& iMesh,
    const uint32_t iMaxNumberOfIndices, 
    std::vector<Mesh> *opMeshes)
{
    assert(opMeshes != nullptr);

    int numberOfVerticesPerFace = iMesh.getNumberOfVerticesPerFace();
    int numFaces = iMesh.getNumberOfFaces();
    uint32_t indexCount = 0;

    // add a mesh in the vector
    opMeshes->push_back(Geometry::Mesh());
    Mesh *pCurrentMesh = &(*opMeshes).back();
    pCurrentMesh->setNumberOfVerticesPerFace( numberOfVerticesPerFace );
    for (int faceIndex = 0; faceIndex < numFaces; ++faceIndex)
    {
        if ((indexCount + numberOfVerticesPerFace) >= iMaxNumberOfIndices)
        {
            indexCount = 0;
            // add a new mesh
            opMeshes->push_back(Geometry::Mesh());
            pCurrentMesh = &(*opMeshes).back();
            pCurrentMesh->setNumberOfVerticesPerFace( numberOfVerticesPerFace );
        }

        // copy all vertexData on current face
        for (int i = 0; i < numberOfVerticesPerFace; ++i)
        {
            pCurrentMesh->getVerticesRef().push_back( iMesh.getVertexOnFace(i, faceIndex) );
        }

        //make a face out of this data
        vector<uint32_t> vertexIndicesForFace(numberOfVerticesPerFace);
        for (int i = 0; i < numberOfVerticesPerFace; ++i)
        {
            vertexIndicesForFace[i] = indexCount + i;
        }
        pCurrentMesh->makeFace( vertexIndicesForFace );
        

        indexCount += numberOfVerticesPerFace;
    }
}

//-----------------------------------------------------------------------------
Vector3 Mesh::getCenterPositionOfFace(int iFaceIndex) const
{
    Vector3 r;

    if (iFaceIndex >= 0 && iFaceIndex < (int)mFaces.size())
    {
        const Face& f = mFaces[iFaceIndex];
        
        for (size_t i = 0; i < f.mVertexIndices.size(); ++i)
        {
            r += mVertexData[ f.mVertexIndices[i] ].mVertex;
        }

        r /= (double)std::max((int)f.mVertexIndices.size(), 1);
    }

    return r;
}


//-----------------------------------------------------------------------------
vector<Mesh::Face>& Mesh::getFacesRef()
{
	return mFaces;
}

//-----------------------------------------------------------------------------
const vector<Mesh::Face>& Mesh::getFaces() const
{
	return mFaces;
}

//-----------------------------------------------------------------------------
// Will return the first found face index using the vertex index iVertexIndex.
// returns -1 if nothing found.
//
int Mesh::getFaceIndexUsingVertexIndex(uint32_t iVertexIndex) const
{
    int r = -1;
   
    for (size_t i = 0; i < mFaces.size() && r == -1; ++i)
    {
        const Face& f = mFaces[i];
        for (size_t j = 0; j < f.mVertexIndices.size(); ++j)
        {
            if (f.mVertexIndices[j] == iVertexIndex)
            {
                r = (int)i;
            }
        }
    }

    return r; 
}

//-----------------------------------------------------------------------------
int Mesh::getNumberOfFaces() const
{
	return (int)mFaces.size();
}

//-----------------------------------------------------------------------------
int Mesh::getNumberOfTextureCoordinateLayers() const
{
    int r = 0;
    if (hasTextureCoordinateLayers())
    {
        r = (int)mVertexData.at(0).mLayerIndexToTextureCoordinates.size();
    }
    return r;
}

//-----------------------------------------------------------------------------
int Mesh::getNumberOfVertices() const
{
	return (int)mVertexData.size();
}

//-----------------------------------------------------------------------------
int Mesh::getNumberOfVerticesPerFace() const
{
	return mNumberOfVerticesPerFace;
}

//-----------------------------------------------------------------------------
const Mesh::TangentSpaceData& Mesh::getTangentSpaceData(int iIndex) const
{ 
    const TangentSpaceData *r = &mDummyTangentSpaceData;
    assert(iIndex >= 0 && iIndex < (int)mTangentSpaceData.size());

    if (iIndex >= 0 && iIndex < (int)mTangentSpaceData.size())
    {
        r = &mTangentSpaceData[iIndex];
    }

    return *r;
}

//-----------------------------------------------------------------------------
const vector<Mesh::TangentSpaceData>& Mesh::getTangentSpaceData() const
{ return mTangentSpaceData; }

//-----------------------------------------------------------------------------
vector<Mesh::TangentSpaceData>& Mesh::getTangentSpaceDataRef()
{ return mTangentSpaceData; }

//-----------------------------------------------------------------------------
const Mesh::TangentSpaceData& Mesh::getTangentSpaceDataOnFace(int iVertexIndex, int iFaceIndex) const
{
    const int index = getVertexIndexOnFace(iVertexIndex, iFaceIndex);
    return getTangentSpaceData(index);
}

//-----------------------------------------------------------------------------
vector<int> Mesh::getTextureCoordinateLayerIndices() const
{
    vector<int> r;
        
    if (getNumberOfVertices() > 0)
    {
        const VertexData& vd = mVertexData[0];
        for (auto it = vd.mLayerIndexToTextureCoordinates.begin(); it != vd.mLayerIndexToTextureCoordinates.end(); ++it)
        {
            r.push_back(it->first);
        }
    }

    return r;
}

//-----------------------------------------------------------------------------
const Mesh::VertexData& Mesh::getVertex(int iIndex) const
{
    const VertexData *r = &mDummyVertexData;
    //assert(iIndex >= 0 && iIndex < (int)mVertexData.size());

    if (iIndex >= 0 && iIndex < (int)mVertexData.size())
    {
        r = &mVertexData[iIndex];
    }

	return *r;
}

//-----------------------------------------------------------------------------
// Returns vertex iVertexIndex on face iFaceIndex.
//
const Mesh::VertexData& Mesh::getVertexOnFace(int iVertexIndex, int iFaceIndex) const
{
    const int index = getVertexIndexOnFace(iVertexIndex, iFaceIndex);
    return getVertex(index);
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
const vector<Mesh::VertexData>& Mesh::getVertices() const
{
	return mVertexData;
}

//-----------------------------------------------------------------------------
vector<Mesh::VertexData>& Mesh::getVerticesRef()
{
    return mVertexData;
}

//-----------------------------------------------------------------------------
bool Mesh::hasTangentSpaceData() const
{ return mTangentSpaceData.size() > 0; }

//-----------------------------------------------------------------------------
bool Mesh::hasTextureCoordinateLayers() const
{
    bool r = false;
    if (getNumberOfVertices() > 0)
    {
        r = mVertexData.at(0).mLayerIndexToTextureCoordinates.size() > 0;
    }
    return r;
}

//-----------------------------------------------------------------------------
bool Mesh::hasTextureCoordinateLayer(int iLayerIndex) const
{
    // at least one vertexData
    // Must have an entry with values
    bool r = false;

    if (getNumberOfVertices() > 0)
    {
        const VertexData &vd = mVertexData[0];
        auto it = vd.mLayerIndexToTextureCoordinates.find(iLayerIndex);
        r = it != vd.mLayerIndexToTextureCoordinates.end();
    }

    return r;
}

//-----------------------------------------------------------------------------
// This is a convenient method to create a face and compute a normal for that
// face...
// Returns a face from 3 vertex indices. The convention for normal is right hand.
//
int Mesh::makeFace(const vector<uint32_t>& iVertexIndices, bool iDuplicateVertex /*=false*/)
{
    vector<uint32_t> vertexIndices = iVertexIndices;
    vector<Mesh::VertexData>& vertices = getVerticesRef();
    if (iDuplicateVertex)
    {
        // search in current faces if the vertex index is currently in use,
        // if so, duplicate the vertex
        for (size_t i = 0; i < iVertexIndices.size(); ++i)
        {
            bool foundInCurrentFaces = false;
            for (int faceIndex = 0; faceIndex < mFaces.size() && !foundInCurrentFaces; ++faceIndex)
            {
                for (int vertexIndex = 0; vertexIndex < mFaces[faceIndex].mVertexIndices.size() && !foundInCurrentFaces; ++vertexIndex)
                {
                    foundInCurrentFaces = iVertexIndices[i] == mFaces[faceIndex].mVertexIndices[vertexIndex];
                }
            }

            if(foundInCurrentFaces)
            {
                vertexIndices[i] = (uint16_t)mVertexData.size();
                mVertexData.push_back( VertexData(vertices[iVertexIndices[i]]) );
            }
        }
    }

    Mesh::Face face;
    face.mVertexIndices = vertexIndices;
       
    VertexData& v0 = vertices[face.mVertexIndices[0]];
    VertexData& v1 = vertices[face.mVertexIndices[1]];
    VertexData& v2 = vertices[face.mVertexIndices[2]];

    Math::Vector3 normal = ((v1.mVertex - v0.mVertex) ^ (v2.mVertex - v0.mVertex)).normalize();

    //assign the same normal to each vertices of the face.
   const uint16_t numVertices = (uint16_t)face.mVertexIndices.size();
   for (uint16_t i = 0; i < numVertices; ++i)
   {
       vertices[face.mVertexIndices[i]].mNormal = normal;
   }

   mFaces.push_back(face);

   //returns the face index
   return (int)mFaces.size() - 1;
}

//-----------------------------------------------------------------------------
int Mesh::makeFace(uint32_t i0, uint32_t i1, uint32_t i2, bool iDuplicateVertex /*= false*/)
{
    return makeFace({i0, i1, i2}, iDuplicateVertex);
}

//-----------------------------------------------------------------------------
void Mesh::setNumberOfVerticesPerFace(int iN)
{
	mNumberOfVerticesPerFace = iN;
}

//-----------------------------------------------------------------------------
void Mesh::triangulate()
{
    //-- early out
    if(getNumberOfVerticesPerFace() == 3) return;

    // triangulate all faces..
    std::vector<Mesh::Face> tempFaces;
    for (int fIndex = 0; fIndex < mFaces.size(); ++fIndex)
    {
        const Mesh::Face &face = mFaces[fIndex];
        const uint16_t numVertices = (uint16_t)face.mVertexIndices.size();
        // break face into multiple face...
        const int numFaces = (numVertices - 3) + 1;

        for (int i = 0; i < numFaces; ++i)
        {
            Geometry::Mesh::Face f;
            f.mVertexIndices = {face.mVertexIndices[0], face.mVertexIndices[i+1], face.mVertexIndices[i+2]};
            tempFaces.push_back(f);
        }
    }

    mFaces = tempFaces;

    setNumberOfVerticesPerFace(3);
}