
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
// Le code est fonctionnel, mais bien trop compliqué...
// devrait plutot faire une grille 2d homogene, mesher et deplacer les points
// avec la formule:
//		vd.mVertex = Vector3(
//			ellipse.x() * sinTheta * sinPhi,
//			ellipse.y() * cosPhi,
//			ellipse.z() * cosTheta * sinPhi);
//
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
	//add north pole, we add as many point as longitudinal stacks to have a nice uv interpolation
    Mesh::VertexData vd;
	for (int j = 0; j <= numLongStacks; ++j)
	{
		vd.mVertex = Vector3(0, ellipse.y(), 0);
		vd.mLayerIndexToTextureCoordinates[0] = Vector2(j / (float)numLongStacks, 0.0);
		vd.mNormal = vd.mVertex; vd.mNormal.normalize();
		vertices.push_back(vd);
	}

	for (int i = 1; i < numLatStacks; ++i)
	{
		const double phi = M_PI * ((double)i / numLatStacks);
		const double sinPhi = sin(phi);
		const double cosPhi = cos(phi);

		// for texture coords to go from 0.0 to 1.0, we need to duplicate the start/end point.
		// that explains the j <= numLongStacks
		for (int j = 0; j <= numLongStacks; ++j)
		{
			const double theta = (2.0 * M_PI) * (double)j / numLongStacks;
			const double sinTheta = sin(theta);
			const double cosTheta = cos(theta);

            vd.mVertex = Vector3(
                ellipse.x() * sinTheta * sinPhi,
                ellipse.y() * cosPhi,
				ellipse.z() * cosTheta * sinPhi);
			vd.mNormal = vd.mVertex; vd.mNormal.normalize();
			vd.mLayerIndexToTextureCoordinates[0] = Vector2(j / (float)numLongStacks, -i / (float)numLatStacks );
            vertices.push_back(vd);

		}
	}
		
	for (int j = 0; j <= numLongStacks; ++j)
	{
		vd.mVertex = Vector3(0, -ellipse.y(), 0);
		vd.mLayerIndexToTextureCoordinates[0] = Vector2(j / (float)numLongStacks, -1.0);
		vd.mNormal = vd.mVertex; vd.mNormal.normalize();
		vertices.push_back(vd);
	}

	//--- mesh vertices together into faces...		
	//north pole ring
	const int northPoleIndexOffset = numLongStacks+1;
	for (int i = 0; i <= numLongStacks; ++i)
	{
		const int northPoleIndex = i;
		const int vertexIndex = northPoleIndexOffset + i;
		const int nextVertexIndex = northPoleIndexOffset + (i + 1);
		mMesh.makeFace({ (uint16_t)northPoleIndex, (uint16_t)vertexIndex, (uint16_t)nextVertexIndex } );
	}

	// all rings in between
	// minus 1 on the latStacks because the last stack is handled separately with
	// the south pole point
	//
	for (int i = 1; i < numLatStacks-1; ++i)
	{
		const int upperStackIndexOffset = (numLongStacks+1) * (i - 1) + northPoleIndexOffset; //+ northPoleIndexOffset because of north pole vertices
		const int lowerStackIndexOffset = (numLongStacks+1) * i + northPoleIndexOffset; //+ northPoleIndexOffset because of north pole vertices
		
		for (int j = 0; j < numLongStacks; ++j)
		{	
			const int upperStackVertexIndex = upperStackIndexOffset + j;
			const int upperStackNextVertexIndex = upperStackIndexOffset + (j + 1);

			const int lowerStackVertexIndex = lowerStackIndexOffset + j;
			const int lowerStackNextVertexIndex = lowerStackIndexOffset + (j + 1);
			
			mMesh.makeFace({ (uint16_t)upperStackVertexIndex, (uint16_t)lowerStackVertexIndex, (uint16_t)lowerStackNextVertexIndex });

			mMesh.makeFace({ (uint16_t)upperStackNextVertexIndex, (uint16_t)upperStackVertexIndex, (uint16_t)lowerStackNextVertexIndex });
		}
	}

	//south pole ring
	const int upperStackIndexOffset = mMesh.getNumberOfVertices() - (numLongStacks+1) - (numLongStacks + 1);
	const int southPoleOffset = numLongStacks + 1;
	for (int i = 0; i < numLongStacks; ++i)
	{
		
		const int upperStackVertexIndex = upperStackIndexOffset + i;
		const int upperStackNextVertexIndex = upperStackIndexOffset + (i + 1);
		const int southPoleIndex = mMesh.getNumberOfVertices() - southPoleOffset + i;

		mMesh.makeFace({ (uint16_t)southPoleIndex, (uint16_t)upperStackNextVertexIndex, (uint16_t)upperStackVertexIndex });
	}

	//mMesh.generateFlatNormals();
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
