
#include <algorithm>
#include <cmath>
#include "GeometricGrid.h"

using namespace Realisim;
    using namespace Geometry;
	using namespace Math;
using namespace std;

GeometricGrid::GeometricGrid() :
	mEllipsoidRadii(),
	mNumberOfLatitudinalStacks(0),
    mNumberOfLongitudinalStacks(0),
	mMesh()
{}

//--------------------------------------------------------------------------
GeometricGrid::~GeometricGrid()
{}

//--------------------------------------------------------------------------
void GeometricGrid::bakeMesh()
{
	mMesh.setNumberOfVerticesPerFace(3);

	//-- grab references to guts of mesh
	vector<Vector3>& vertices = mMesh.getVertices();
	vector<Mesh::Face>& faces = mMesh.getFaces();
	vector<int>& triangulatedFaceIndices = mMesh.getFaceIndices();

	const int numLatStacks = getNumberOfLatitudinalStacks();
	const int numLongStacks = getNumberOfLongitudinalStacks();
	vertices.reserve(numLatStacks * numLongStacks + 2);

	Vector3 ellipse = getEllipsoidRadii();

	// create all vertices...
	//add north pole
	vertices.push_back(Vector3(0, 0, ellipse.z()));

	for (int i = 1; i < numLatStacks; ++i)
	{
		const double phi = M_PI * ((double)i / numLatStacks);
		const double sinPhi = sin(phi);
		const double cosPhi = cos(phi);

		for (int j = 0; j < numLongStacks; ++j)
		{
			const double theta = (2.0 * M_PI) * (double)j / numLongStacks;
			const double sinTheta = sin(theta);
			const double cosTheta = cos(theta);

			vertices.push_back(Vector3(
				ellipse.x() * cosTheta * sinPhi,
				ellipse.y() * sinTheta * sinPhi,
				ellipse.z() * cosPhi));

		}
	}
		
	//add south pole
	vertices.push_back(Vector3(0, 0, -ellipse.z()));

	//--- mesh vertices together into faces...		
	//north pole ring
	const int northPoleIndex = 0;
	const int southPoleIndex = mMesh.getNumberOfVertices() - 1;
	for (int i = 0; i < numLongStacks; ++i)
	{
		const int vertexIndex = (i % numLongStacks) + 1;
		const int nextVertexIndex = ((i + 1) % numLongStacks) + 1;
		Mesh::Face f = makeFace(northPoleIndex, vertexIndex, nextVertexIndex);		
		faces.push_back(f);
	}

	// all rings in between
	// minus 1 on the latStacks because the last stack is handled separately with
	// the south pole point
	//
	for (int i = 1; i < numLatStacks-1; ++i)
	{
		const int upperStackIndexOffset = numLongStacks * (i - 1) + 1; //+ 1 because of north pole vertex
		const int lowerStackIndexOffset = numLongStacks * i + 1; //+ 1 because of north pole vertex
		
		for (int j = 0; j < numLongStacks; ++j)
		{	
			const int upperStackVertexIndex = upperStackIndexOffset + (j % numLongStacks);
			const int upperStackNextVertexIndex = upperStackIndexOffset + ((j + 1) % numLongStacks);

			const int lowerStackVertexIndex = lowerStackIndexOffset + (j % numLongStacks);
			const int lowerStackNextVertexIndex = lowerStackIndexOffset + ((j + 1) % numLongStacks);
			
			Mesh::Face f = makeFace(upperStackVertexIndex, lowerStackVertexIndex, lowerStackNextVertexIndex);			
			faces.push_back(f);

			f = makeFace(upperStackNextVertexIndex, upperStackVertexIndex, lowerStackNextVertexIndex);
			faces.push_back(f);	
		}
	}

	//south pole ring
	const int upperStackIndexOffset = southPoleIndex - numLongStacks;
	for (int i = 0; i < numLongStacks; ++i)
	{
		const int upperStackVertexIndex = upperStackIndexOffset + (i % numLongStacks);
		const int upperStackNextVertexIndex = upperStackIndexOffset + ((i + 1) % numLongStacks);

		Mesh::Face f = makeFace(southPoleIndex, upperStackNextVertexIndex, upperStackVertexIndex);
		faces.push_back(f);
	}

	//--- create indices..
	triangulatedFaceIndices.resize(mMesh.getNumberOfFaces() * mMesh.getNumberOfVerticesPerFace());
	for (size_t i = 0; i < triangulatedFaceIndices.size(); ++i)
	{
		triangulatedFaceIndices[i] = (int)i;
	}
}

//--------------------------------------------------------------------------
void GeometricGrid::clear()
{
	mEllipsoidRadii = Vector3();
	mNumberOfLongitudinalStacks = 0;
	mNumberOfLatitudinalStacks = 0;
	mMesh.clear();
}

//--------------------------------------------------------------------------
Math::Vector3 GeometricGrid::getEllipsoidRadii() const
{
	return mEllipsoidRadii;
}

//--------------------------------------------------------------------------
const Mesh& GeometricGrid::getMesh() const
{
	return mMesh;
}

//--------------------------------------------------------------------------
int GeometricGrid::getNumberOfLatitudinalStacks() const
{
	return mNumberOfLatitudinalStacks;
}

//--------------------------------------------------------------------------
int GeometricGrid::getNumberOfLongitudinalStacks() const
{
	return mNumberOfLongitudinalStacks;
}

//--------------------------------------------------------------------------
Mesh::Face GeometricGrid::makeFace(int iIndex0, 
	int iIndex1,
	int iIndex2)
{
	Mesh::Face f;
	f.mVertexIndices.resize(mMesh.getNumberOfVerticesPerFace());
	f.mVertexIndices[0] = iIndex0;
	f.mVertexIndices[1] = iIndex1;
	f.mVertexIndices[2] = iIndex2;

	const vector<Vector3>& vertices = mMesh.getVertices();

	f.mNormal = (vertices.at(iIndex1) - vertices.at(iIndex0)) ^
		(vertices.at(iIndex2) - vertices.at(iIndex0)).normalize();

	return f;
}

//--------------------------------------------------------------------------
void GeometricGrid::set(const Math::Vector3& iEllipsoidRadius, int iNumberOfLatitudinalStacks, int iNumberOfLongitudinalStacks)
{
	clear();
	mEllipsoidRadii = iEllipsoidRadius;
	mNumberOfLatitudinalStacks = std::max(iNumberOfLatitudinalStacks, 2);
	if (mNumberOfLatitudinalStacks % 2 != 0)
	{
		mNumberOfLatitudinalStacks += 1;
	}

	mNumberOfLongitudinalStacks = std::max(iNumberOfLongitudinalStacks, 3);

	if (mEllipsoidRadii.norm() > 0)
	{
		bakeMesh();
	}
}
