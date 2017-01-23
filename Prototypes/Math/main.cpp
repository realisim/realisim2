
#include <iostream>
#include "Math/Index.h"
#include "Math/Matrix.h"
#include "Math/Quaternion.h"
#include "Math/Vector.h"

using namespace Realisim;
using namespace Math;

//-----------------------------------------------------------
void testIndex2()
{}

//-----------------------------------------------------------
void testIndex3()
{}

//-----------------------------------------------------------
void testVector2()
{
    printf("\n--- testVector2 --- \n");
    
    Vector2 v;
    printf("%s\n", v.toString().c_str());
}

//-----------------------------------------------------------
void testVector3()
{}

//-----------------------------------------------------------
void testVector4()
{}

//-----------------------------------------------------------
void testMatrix4()
{}

//-----------------------------------------------------------
void testQuaternion()
{}

//-----------------------------------------------------------
int main(int argc, char** argv)
{
    testIndex2();
    testIndex3();
    
    testVector2();
    testVector3();
    testVector4();
    
    testMatrix4();
    
    testQuaternion();
    
    return 0;
}
