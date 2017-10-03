#pragma once

#include "Math/Vector.h"
#include "Mesh.h"

namespace Realisim
{
namespace Geometry
{
	/*	This class represent a very basic geometric grid to represent the earth.
		It is very similar to gluSphere. The difference is that the radius can
		be specified per axis, giving the possibillity to represent an ellipsoid.

		The number of longitudinal and latitudinal stacks can be specified.

		The GeometricGrid creates a mesh when set() method is invoked.
	*/
    class GeometricGrid
    {
    public:
        GeometricGrid();
        GeometricGrid(const GeometricGrid&) = default;
        GeometricGrid& operator=(const GeometricGrid&) = default;
        ~GeometricGrid();

        void clear();
		Math::Vector3 getEllipsoidRadii() const;
		const Mesh& getMesh() const;
        int getNumberOfLatitudinalStacks() const;
        int getNumberOfLongitudinalStacks() const;
        void set(const Math::Vector3& iEllipsoidRadius, int iNumberOfLatitudinalStacks, int iNumberOfLongitudinalStacks);

    protected:
		Mesh::Face makeFace(int iIndex0, int iIndex1, int iIndex2);
		void bakeMesh();

		Math::Vector3 mEllipsoidRadii; // meters
		int mNumberOfLatitudinalStacks;
        int mNumberOfLongitudinalStacks;
		Mesh mMesh;
    };
}
}