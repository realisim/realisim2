
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
	vector<Mesh::VertexData>& vertices = mMesh.getVerticesRef();

	const int numLatStacks = getNumberOfLatitudinalStacks();
	const int numLongStacks = getNumberOfLongitudinalStacks();
	vertices.reserve(numLatStacks * numLongStacks + 2);

	Vector3 ellipse = getEllipsoidRadii();

	// create all vertices...
	//add north pole
    Mesh::VertexData vd;
    vd.mVertex = Vector3(0, 0, ellipse.z());
	vertices.push_back(vd);

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

            vd.mVertex = Vector3(Vector3(
                ellipse.x() * cosTheta * sinPhi,
                ellipse.y() * sinTheta * sinPhi,
                ellipse.z() * cosPhi));
            vertices.push_back(vd);

		}
	}
		
	//add south pole
    vd.mVertex = Vector3(0, 0, -ellipse.z());
    vertices.push_back(vd);

	//--- mesh vertices together into faces...		
	//north pole ring
	const int northPoleIndex = 0;
	const int southPoleIndex = mMesh.getNumberOfVertices() - 1;
	for (int i = 0; i < numLongStacks; ++i)
	{
		const int vertexIndex = (i % numLongStacks) + 1;
		const int nextVertexIndex = ((i + 1) % numLongStacks) + 1;
		mMesh.makeFaceB({ (uint16_t)northPoleIndex, (uint16_t)vertexIndex, (uint16_t)nextVertexIndex } );
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
			
			mMesh.makeFaceB({ (uint16_t)upperStackVertexIndex, (uint16_t)lowerStackVertexIndex, (uint16_t)lowerStackNextVertexIndex });

			mMesh.makeFaceB({ (uint16_t)upperStackNextVertexIndex, (uint16_t)upperStackVertexIndex, (uint16_t)lowerStackNextVertexIndex });
		}
	}

	//south pole ring
	const int upperStackIndexOffset = southPoleIndex - numLongStacks;
	for (int i = 0; i < numLongStacks; ++i)
	{
		const int upperStackVertexIndex = upperStackIndexOffset + (i % numLongStacks);
		const int upperStackNextVertexIndex = upperStackIndexOffset + ((i + 1) % numLongStacks);

		mMesh.makeFaceB({ (uint16_t)southPoleIndex, (uint16_t)upperStackNextVertexIndex, (uint16_t)upperStackVertexIndex });
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
