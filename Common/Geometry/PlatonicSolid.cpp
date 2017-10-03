
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
	vector<Vector3>& vertices = mMesh.getVertices();
	vector<Mesh::Face>& faces = mMesh.getFaces();
	vector<int>& triangulatedFaceIndices = mMesh.getFaceIndices();

	vertices.resize(8);
    //+Z
	vertices[0] = Vector3(-1, -1, 1); //0
	vertices[1] = Vector3(1, -1, 1);  //1
	vertices[2] = Vector3(1, 1, 1);   //2
	vertices[3] = Vector3(-1, 1, 1);  //3

    //-Z
	vertices[4] = Vector3(-1, -1, -1); //4
	vertices[5] = Vector3(1, -1, -1);  //5
	vertices[6] = Vector3(1, 1, -1);   //6
	vertices[7] = Vector3(-1, 1, -1);  //7

    // 6 faces with 4 vertex each... so 24 entries
	faces.resize(6, Mesh::Face());
    for (int i = 0; i < mMesh.getNumberOfFaces(); ++i)
    {
		faces[i].mVertexIndices.resize(mMesh.getNumberOfVerticesPerFace());
    }

	faces[0] = makeFace({ 0, 1, 2, 3 }); //0, 1, 2, 3
	faces[1] = makeFace({ 4, 7, 6, 5 }); //4, 5, 6, 7
	faces[2] = makeFace({ 1, 5, 6, 2 }); //8, 9, 10, 11  
	faces[3] = makeFace({ 0, 3, 7, 4 }); //12, 13, 14, 15
	faces[4] = makeFace({ 2, 6, 7, 3 }); //16, 17, 18, 19  
	faces[5] = makeFace({ 0, 4, 5, 1 }); //20 21 22 23

    // 12 faces with 3 vertex each
	triangulatedFaceIndices = {
        //Z
        0, 1, 2,
        2, 3, 0,

        //-Z
        4, 5, 6,
        6, 7, 4,

        //X
        8, 9, 10,
        10, 11, 8,

        //-x
        12, 13, 14,
        14, 15, 12,

        //y
        16, 17, 18,
        18, 19, 16,

        //-y
        20, 21, 22,
        22, 23, 20
    };    
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
	vector<Vector3>& vertices = mMesh.getVertices();
	vector<Mesh::Face>& faces = mMesh.getFaces();
	vector<int>& triangulatedFaceIndices = mMesh.getFaceIndices();

    const double phi = (1 + sqrt(5.0) ) / 2.0;
    const double oneOverPhi = 1.0 / phi;
	vertices = {

        // the orange dot on the wiki page.
        //Z
        Vector3(-1, -1, 1),  //0
        Vector3( 1, -1, 1),  //1
        Vector3( 1,  1, 1),  //2
        Vector3(-1,  1, 1),  //3
        //-Z
        Vector3(-1, -1, -1), //4
        Vector3( 1, -1, -1), //5
        Vector3( 1, 1, -1),  //6
        Vector3(-1, 1, -1),  //7

        // green vertices on wiki
        Vector3(0,  phi,  oneOverPhi), //8
        Vector3(0, -phi,  oneOverPhi), //9
        Vector3(0, -phi, -oneOverPhi), //10
        Vector3(0,  phi, -oneOverPhi), //11

        // blue vertices on wiki
        Vector3( oneOverPhi, 0, phi), //12
        Vector3(-oneOverPhi, 0, phi), //13
        Vector3( -oneOverPhi,0, -phi), //14
        Vector3( oneOverPhi, 0, -phi), //15

        // pink vertices on wiki
        Vector3( phi,  oneOverPhi, 0), //16
        Vector3(-phi,  oneOverPhi, 0), //17
        Vector3(-phi, -oneOverPhi, 0), //18
        Vector3( phi, -oneOverPhi, 0) //19
    };


    // 12 faces with 5 vertex each... so 60 entries
	faces.resize(12, Mesh::Face());
    for (int i = 0; i < mMesh.getNumberOfFaces(); ++i)
    {
		faces[i].mVertexIndices.resize(mMesh.getNumberOfVerticesPerFace());
    }

	faces[0] = makeFace({ 1, 9, 10, 5, 19 }); // 0, 1, 2, 3, 4
	faces[1] = makeFace({ 1, 19, 16, 2, 12 }); //5 6 7 8 9
	faces[2] = makeFace({ 0, 9, 1, 12, 13 }); //10 11 12 13 14
	faces[3] = makeFace({ 0, 18, 4, 10, 9 }); //15 16 17 18 19
	faces[4] = makeFace({ 4, 14, 15, 5, 10}); //20 21 22 23 24
	faces[5] = makeFace({ 5, 15, 6, 16, 19}); //25 26, 27, 28, 29
	faces[6] = makeFace({ 3, 8, 11, 7, 17 }); //30 31 32 33 34
	faces[7] = makeFace({ 2, 8, 3, 13, 12 }); //35 36 37 38 39
	faces[8] = makeFace({ 3, 17, 18, 0, 13}); //40 41 42 43 44
	faces[9] = makeFace({ 4, 18, 17, 7, 14}); //15 46 47 48 49
	faces[10] = makeFace({ 15, 14, 7, 11, 6}); //50 51 52 53 54
	faces[11] = makeFace({ 2, 16, 6, 11, 8 }); //55 56 57 58 59 

    // 12 faces with 3 triangles with 3 vertex each
    // 12 * 3 * 3 = 96
	triangulatedFaceIndices = {
        //a
        0, 1, 2,
        0, 2, 3,
        0, 3, 4,

        //b
        5, 6, 7,
        5, 7, 8,
        5, 8, 9,

        //c
        10, 11, 12,
        10, 12, 13,
        10, 13, 14,

        //d
        15, 16, 17,
        15, 17, 18,
        15, 18, 19,

        //e
        20, 21, 22,
        20, 22, 23,
        20, 23, 24,

        //f
        25, 26, 27,
        25, 27, 28,
        25, 28, 29,

        //g
        30, 31, 32,
        30, 32, 33,
        30, 33, 34,

        //h
        35, 36, 37,
        35, 37, 38,
        35, 38, 39,

        //i
        40, 41, 42,
        40, 42, 43,
        40, 43, 44,

        //j
        45, 46, 47,
        45, 47, 48,
        45, 48, 49,

        //k
        50, 51, 52,
        50, 52, 53,
        50, 53, 54,

        //l
        55, 56, 57,
        55, 57, 58,
        55, 58, 59
    };
}

//-----------------------------------------------------------------------------
void PlatonicSolid::bakeOctahedron()
{
    // 6 vertices
    // 8 faces
    // 3 vertices per face

	mMesh.setNumberOfVerticesPerFace(3);
	//-- grab references to guts of mesh
	vector<Vector3>& vertices = mMesh.getVertices();
	vector<Mesh::Face>& faces = mMesh.getFaces();
	vector<int>& triangulatedFaceIndices = mMesh.getFaceIndices();

	vertices.resize(6);
    //+Z
	vertices[0] = Vector3(-1,  0,  0); //A
	vertices[1] = Vector3( 1,  0,  0); //B
	vertices[2] = Vector3( 0, -1,  0); //C
	vertices[3] = Vector3( 0,  1,  0); //D
	vertices[4] = Vector3( 0,  0, -1); //E
	vertices[5] = Vector3( 0,  0,  1); //F

    // 8 faces with 3 vertex each... so 24 entries
	faces.resize(8, Mesh::Face());
    for (int i = 0; i < mMesh.getNumberOfFaces(); ++i)
    {
		faces[i].mVertexIndices.resize(mMesh.getNumberOfVerticesPerFace());
    }

	faces[0] = makeFace({ 0, 5, 3 }); // 0, 1, 2
	faces[1] = makeFace({ 1, 3, 5 }); // 3, 4, 5
	faces[2] = makeFace({ 1, 4, 3 }); // 6, 7, 8
	faces[3] = makeFace({ 0, 3, 4 }); //9, 10, 11
	faces[4] = makeFace({ 0, 2, 5 }); //12, 13, 14
	faces[5] = makeFace({ 1, 5, 2 }); //15, 16, 17
	faces[6] = makeFace({ 1, 2, 4 }); //18, 19, 20
	faces[7] = makeFace({ 0, 4, 2 }); //21, 22, 23

    // 8 faces with 3 vertex each
	triangulatedFaceIndices = {
        //a
        0, 1, 2,
        //b
        3, 4, 5,
        //c
        6, 7, 8,
        //d
        9, 10, 11,
        //e
        12, 13, 14,
        //f
        15, 16, 17,
        //g
        18, 19, 20,
        //h
        21, 22, 23
    };    
}

//-----------------------------------------------------------------------------
void PlatonicSolid::bakeTetrahedron()
{
    // 4 vertices
    // 4 faces
    // 3 vertices per face

	mMesh.setNumberOfVerticesPerFace(3);
	//-- grab references to guts of mesh
	vector<Vector3>& vertices = mMesh.getVertices();
	vector<Mesh::Face>& faces = mMesh.getFaces();
	vector<int>& triangulatedFaceIndices = mMesh.getFaceIndices();
    const double square2 = sqrt(2.0);

	vertices.resize(4);
	vertices[0] = Vector3(1, 0, - 1/square2);
	vertices[1] = Vector3(-1, 0, -1 / square2);
	vertices[2] = Vector3(0, 1, 1 / square2);
	vertices[3] = Vector3(0, -1, 1 / square2);

    // 4 faces with 3 vertex each...
	faces.resize(4, Mesh::Face());
    for (int i = 0; i < mMesh.getNumberOfFaces(); ++i)
    {
		faces[i].mVertexIndices.resize(mMesh.getNumberOfVerticesPerFace());
    }
    
	faces[0] = makeFace({ 0, 1, 2 });
	faces[1] = makeFace({ 1, 3, 2 });
	faces[2] = makeFace({ 2, 3, 0 });
	faces[3] = makeFace({ 0, 3, 1 });
    
    // 4 faces with 3 vertex each
	triangulatedFaceIndices = {
        0, 1, 2,
        3, 4, 5,
        6, 7, 8,
        9, 10, 11
    };
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
Mesh::Face PlatonicSolid::makeFace(const std::vector<int>& iVertexIndices)
{
	Mesh::Face f;
	f.mVertexIndices = iVertexIndices;

	assert(f.mVertexIndices.size() >= 3);
	const vector<Vector3>& vertices = mMesh.getVertices();

	f.mNormal = (vertices.at(f.mVertexIndices[1]) - vertices.at(f.mVertexIndices[0])) ^
		(vertices.at(f.mVertexIndices[2]) - vertices.at(f.mVertexIndices[0])).normalize();

	return f;
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