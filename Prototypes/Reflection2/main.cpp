

#include "DummyStruct.h"
#include "DebugWriter.h"


#include <stdint.h>
#include <stdio.h>

// THIS CODE IS HIGHLY INSPIRED FROM 
// https://github.com/preshing/FlexibleReflection
//

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    printf("- Reflection -\n");

    DummyStruct ds;
    ds.mUint32 = 32;
    ds.mUint16 = 16;
    ds.mUint8 = 8;
    ds.mVectorOfUint32 = { 0,1,2,3,4,5,6,7,8,9 };

    ds.mInnerData = { 320, 160, 80 };
    
    ds.mInnerDataVector = {
        { 321, 161, 81 },
        { 322, 162, 82 },
        { 323, 163, 83 },
    };


    DebugWriter dw(ds);
    printf("%s\n", dw.getString().c_str());


    return 0;
}

