
#include <cassert>
#include <cmath>
#include "PlatonicSolid.h"
#include <vector>

using namespace Realisim;
    using namespace Geometry;
    using namespace Math;
using namespace std;

//-----------------------------------------------------------------------------
PlatonicSolid::PlatonicSolid() :
    mType(tUndefined)
{
}

//-----------------------------------------------------------------------------
PlatonicSolid::PlatonicSolid(type iType) :
    mType(tUndefined)
{
    set(iType);
}

//-----------------------------------------------------------------------------
PlatonicSolid::~PlatonicSolid()
{
	clear();
}

//-----------------------------------------------------------------------------
void PlatonicSolid::bakeCube()
{
    // 8 vertices
    // 6 faces
    // 4 vertices per face

	mMesh.setNumberOfVerticesPerFace(4); 
	//-- grab references to guts of mesh
	vector<Mesh::VertexData>& vertices = mMesh.getVerticesRef();

	vertices.resize(8);
    //+Z
	vertices[0].mVertex = Vector3(-1, -1, 1); //0
	vertices[1].mVertex = Vector3(1, -1, 1);  //1
	vertices[2].mVertex = Vector3(1, 1, 1);   //2
	vertices[3].mVertex = Vector3(-1, 1, 1);  //3

    //-Z
	vertices[4].mVertex = Vector3(-1, -1, -1); //4
    vertices[5].mVertex = Vector3(1, -1, -1);  //5
    vertices[6].mVertex = Vector3(1, 1, -1);   //6
    vertices[7].mVertex = Vector3(-1, 1, -1);  //7

	mMesh.makeFace({ 0, 1, 2, 3 }, true); //0, 1, 2, 3
	mMesh.makeFace({ 4, 7, 6, 5 }, true); //4, 5, 6, 7
	mMesh.makeFace({ 5, 6, 2, 1 }, true); //8, 9, 10, 11  
	mMesh.makeFace({ 0, 3, 7, 4 }, true); //12, 13, 14, 15
	mMesh.makeFace({ 6, 7, 3, 2 }, true); //16, 17, 18, 19  
	mMesh.makeFace({ 4, 5, 1, 0 }, true); //20 21 22 23

    mMesh.triangulate();
}

//-----------------------------------------------------------------------------
// https://en.wikipedia.org/wiki/Regular_dodecahedron
//
// see also on confluence http://simz800:8090/display/AXION/PlatonicSolid
void PlatonicSolid::bakeDodecahedron()
{
    // 20 vertices
    // 12 faces
    // 5 vertices per face

	mMesh.setNumberOfVerticesPerFace(5);
	//-- grab references to guts of mesh
	vector<Mesh::VertexData>& vertices = mMesh.getVerticesRef();

    const double phi = (1 + sqrt(5.0) ) / 2.0;
    const double oneOverPhi = 1.0 / phi;

    vertices.resize(20);

    // the orange dot on the wiki page.
    //Z
    vertices[0].mVertex = Vector3(-1, -1, 1);  //0
    vertices[1].mVertex = Vector3( 1, -1, 1);  //1
    vertices[2].mVertex = Vector3( 1,  1, 1);  //2
    vertices[3].mVertex = Vector3(-1,  1, 1);  //3
    //-Z
    vertices[4].mVertex = Vector3(-1, -1, -1); //4
    vertices[5].mVertex = Vector3( 1, -1, -1); //5
    vertices[6].mVertex = Vector3( 1, 1, -1); //6
    vertices[7].mVertex = Vector3(-1, 1, -1);  //7

    // green vertices on wiki
    vertices[8].mVertex = Vector3(0,  phi,  oneOverPhi); //8
    vertices[9].mVertex = Vector3(0, -phi,  oneOverPhi); //9
    vertices[10].mVertex = Vector3(0, -phi, -oneOverPhi); //10
    vertices[11].mVertex = Vector3(0,  phi, -oneOverPhi); //11

    // blue vertices on wiki
    vertices[12].mVertex = Vector3( oneOverPhi, 0, phi); //12
    vertices[13].mVertex = Vector3(-oneOverPhi, 0, phi); //13
    vertices[14].mVertex = Vector3( -oneOverPhi,0, -phi); //14
    vertices[15].mVertex = Vector3( oneOverPhi, 0, -phi); //15

    // pink vertices on wiki
    vertices[16].mVertex = Vector3( phi,  oneOverPhi, 0); //16
    vertices[17].mVertex = Vector3(-phi,  oneOverPhi, 0); //17
    vertices[18].mVertex = Vector3(-phi, -oneOverPhi, 0); //18
    vertices[19].mVertex = Vector3( phi, -oneOverPhi, 0); //19

    //12 faces
	mMesh.makeFace({ 1, 9, 10, 5, 19 }, true); // 0, 1, 2, 3, 4
	mMesh.makeFace({ 1, 19, 16, 2, 12 }, true); //5 6 7 8 9
	mMesh.makeFace({ 0, 9, 1, 12, 13 }, true); //10 11 12 13 14
	mMesh.makeFace({ 0, 18, 4, 10, 9 }, true); //15 16 17 18 19
	mMesh.makeFace({ 4, 14, 15, 5, 10}, true); //20 21 22 23 24
	mMesh.makeFace({ 5, 15, 6, 16, 19}, true); //25 26, 27, 28, 29
	mMesh.makeFace({ 3, 8, 11, 7, 17 }, true); //30 31 32 33 34
	mMesh.makeFace({ 2, 8, 3, 13, 12 }, true); //35 36 37 38 39
	mMesh.makeFace({ 3, 17, 18, 0, 13}, true); //40 41 42 43 44
	mMesh.makeFace({ 4, 18, 17, 7, 14}, true); //15 46 47 48 49
	mMesh.makeFace({ 15, 14, 7, 11, 6}, true); //50 51 52 53 54
	mMesh.makeFace({ 2, 16, 6, 11, 8 }, true); //55 56 57 58 59 

    mMesh.triangulate();
}

//-----------------------------------------------------------------------------
void PlatonicSolid::bakeOctahedron()
{
    // 6 vertices
    // 8 faces
    // 3 vertices per face

	mMesh.setNumberOfVerticesPerFace(3);
	//-- grab references to guts of mesh
	vector<Mesh::VertexData>& vertices = mMesh.getVerticesRef();

	vertices.resize(6);
    //+Z
	vertices[0].mVertex = Vector3(-1,  0,  0); //A
	vertices[1].mVertex = Vector3( 1,  0,  0); //B
	vertices[2].mVertex = Vector3( 0, -1,  0); //C
	vertices[3].mVertex = Vector3( 0,  1,  0); //D
	vertices[4].mVertex = Vector3( 0,  0, -1); //E
	vertices[5].mVertex = Vector3( 0,  0,  1); //F

    // 8 faces with 3 vertex each... so 24 entries

	mMesh.makeFace({ 0, 5, 3 }, true); // 0, 1, 2
	mMesh.makeFace({ 1, 3, 5 }, true); // 3, 4, 5
	mMesh.makeFace({ 1, 4, 3 }, true); // 6, 7, 8
	mMesh.makeFace({ 0, 3, 4 }, true); //9, 10, 11
	mMesh.makeFace({ 0, 2, 5 }, true); //12, 13, 14
	mMesh.makeFace({ 1, 5, 2 }, true); //15, 16, 17
	mMesh.makeFace({ 1, 2, 4 }, true); //18, 19, 20
	mMesh.makeFace({ 0, 4, 2 }, true); //21, 22, 23
}

//-----------------------------------------------------------------------------
void PlatonicSolid::bakeTetrahedron()
{
    // 4 vertices
    // 4 faces
    // 3 vertices per face

	mMesh.setNumberOfVerticesPerFace(3);
	//-- grab references to guts of mesh
	vector<Mesh::VertexData>& vertices = mMesh.getVerticesRef();
    const double square2 = sqrt(2.0);

	vertices.resize(4);
	vertices[0].mVertex = Vector3(1, 0, - 1/square2);
	vertices[1].mVertex = Vector3(-1, 0, -1 / square2);
	vertices[2].mVertex = Vector3(0, 1, 1 / square2);
	vertices[3].mVertex = Vector3(0, -1, 1 / square2);

    // 4 faces with 3 vertex each...
	mMesh.makeFace({ 0, 1, 2 }, true);
	mMesh.makeFace({ 1, 3, 2 }, true);
	mMesh.makeFace({ 2, 3, 0 }, true);
	mMesh.makeFace({ 0, 3, 1 }, true);
    
}


//-----------------------------------------------------------------------------
// clears everything.
//
void PlatonicSolid::clear()
{
    mType = tUndefined;
	mMesh.clear();
}

//-----------------------------------------------------------------------------
const Mesh& PlatonicSolid::getMesh() const
{
	return mMesh;
}

//-----------------------------------------------------------------------------
PlatonicSolid::type PlatonicSolid::getType() const
{
    return mType;
}

//-----------------------------------------------------------------------------
void PlatonicSolid::set(type iType)
{
    clear();

    mType = iType;
    
    switch (getType())
    {
    case tTetrahedron: bakeTetrahedron(); break;
    case tCube: bakeCube(); break;
    case tOctahedron: bakeOctahedron(); break;
    case tDodecahedron: bakeDodecahedron(); break;
    default: assert(0);  break;
    }
    
}