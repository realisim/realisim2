#pragma once

#include "Math/Vector.h"

#include "Mesh.h"


namespace Realisim
{
namespace Geometry
{

    /*
        Vertices represents all vertices of the platonic solid.

        Indices represents how to connect the vertices to form triangles
        to would close the surface. The winding of the triangles is 
        counter-clockwise.
    
        reference:
            https://en.wikipedia.org/wiki/Platonic_solid
    */
    class PlatonicSolid
    {
    public:
        enum type{tUndefined, tTetrahedron, tCube, tOctahedron, tDodecahedron};

        PlatonicSolid();
        PlatonicSolid(type iType);
        PlatonicSolid(const PlatonicSolid&) = default;
        PlatonicSolid& operator=(const PlatonicSolid&) = default;
        ~PlatonicSolid();

        void clear();
//double getRadius() const;
//double getVolume() const;
		const Mesh& getMesh() const;
        type getType() const;
        void set(type iType);        

    protected:
        
        void bakeCube();
        void bakeDodecahedron();
        void bakeTetrahedron();
        void bakeOctahedron();
        
        type mType;
		Mesh mMesh;
    };
}
}