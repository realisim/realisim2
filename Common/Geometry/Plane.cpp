
#include "Math/IsEqual.h"
#include "Math/VectorI.h"
#include "Plane.h"
#include <vector>

using namespace Realisim;
	using namespace Geometry;
	using namespace Math;
using namespace std;

namespace
{
	//---------------------------------------------------------------------------
	Vector3 getPerpendicularVector(const Vector3& iV)
	{
		/*afin d'obtenir un vecteur perpendiculaire, le produit scalaire doit donner
		0. donc
		1- (ax, by, cz) * (dx, ey, fz) = 0
		2- ( a*d + b*e + c*z ) = 0
		si d = b et que e = -a et que z = 0,
		3- a*b + b*(-a) + 0 = 0
		Bref, en permuttant deux valeurs et en inversant une des deux et remplacant
		la troisieme par 0, on obtient toujours un vecteur perpendiculaire.*/
		Vector3 r(1.0, 0.0, 0.0);
		if (!isEqual(iV.x(), 0.0))
			r = Vector3(iV.y(), -iV.x(), 0.0);
		else if (!isEqual(iV.y(), 0.0))
			r = Vector3(-iV.y(), iV.x(), 0.0);
		else if (!isEqual(iV.z(), 0.0))
			r = Vector3(0.0, iV.z(), -iV.y());
		return r;
	}
}

//------------------------------------------------------------------------------
Plane::Plane() :
	mIsValid(true),
	mPoint(0.0),
	mNormal(0.0, 1.0, 0.0)
{}

//------------------------------------------------------------------------------
Plane::Plane(const Vector3& iP, const Vector3& iN) :
	mIsValid(false),
	mPoint(iP),
	mNormal(iN)
{
	validate();
}

//------------------------------------------------------------------------------
Plane::Plane(const Math::Vector3& p0,
	const Math::Vector3& p1, 
	const Math::Vector3& p2) :
	mIsValid(false),
	mPoint(0.0),
	mNormal(0.0, 0.0, 0.0)
{
	set(p0, p1, p2);
}

//------------------------------------------------------------------------------
Plane::~Plane()
{}

//------------------------------------------------------------------------------
Vector3 Plane::getPoint() const
{
	return mPoint;
}

//------------------------------------------------------------------------------
Vector3 Plane::getNormal() const
{
	return mNormal;
}

//------------------------------------------------------------------------------
bool Plane::isValid() const
{
	return mIsValid;
}

//------------------------------------------------------------------------------
Mesh Plane::makeMesh(const Vector2& iSizeInMeter) const
{
	Mesh mesh;

	mesh.setNumberOfVerticesPerFace(3);

	//-- grab references to guts of mesh
	vector<Vector3>& vertices = mesh.getVertices();
	vector<Mesh::Face>& faces = mesh.getFaces();
	vector<int>& triangulatedFaceIndices = mesh.getFaceIndices();

	Vector3 someX = getPerpendicularVector(getNormal());
	Vector3 someY = getNormal() ^ someX;
	someX.normalize();
	someY.normalize();

	//4 vertices
	const Vector3& p = getPoint();
	const Vector2 halfSize(iSizeInMeter.x() / 2.0, iSizeInMeter.y() / 2.0);
	vertices = { p - (someX * halfSize.x()) - (someY * halfSize.y()),
		p + (someX * halfSize.x()) - (someY * halfSize.y()),
		p + (someX * halfSize.x()) + (someY * halfSize.y()),
		p - (someX * halfSize.x()) + (someY * halfSize.y())
	};
	
	Mesh::Face f0, f1;
	f0.mVertexIndices = { 0, 1, 2 };
	f0.mNormal = getNormal();
	f1.mVertexIndices = { 2, 3, 0 };
	f1.mNormal = getNormal();
	faces = { f0, f1 };

	triangulatedFaceIndices = { 0, 1, 2, 3, 4, 5, 6 };
	return mesh;
}

//------------------------------------------------------------------------------
void Plane::set(const Math::Vector3& p, const Math::Vector3& n)
{
	mPoint = p;
	mNormal = n;
	validate();
}

//------------------------------------------------------------------------------
void Plane::set(const Math::Vector3& p0,
	const Math::Vector3& p1,
	const Math::Vector3& p2)
{
	Vector3 v0 = p1 - p0;
	Vector3 v1 = p2 - p0;
	set(p0, v0 ^ v1);
}

//-----------------------------------------------------------------------------
void Plane::validate()
{
	mIsValid = !mNormal.isEqual(Vector3(0.0, 0.0, 0.0), 1e-13);
	if (mIsValid)
	{
		mNormal.normalize();
	}
}