

#include "DummyStruct.h"
#include "DebugPrintVisitor.h"
#include "BinaryVisitor.h"


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


    DebugPrintVisitor debugPrinter;
    debugPrinter.visit(ds);
    printf("ds\n %s\n", debugPrinter.getString().c_str());
    debugPrinter.clear();

    BinaryWriterVisitor bWriter;
    BinaryReaderVisitor bReader;

    bWriter.visit(ds);

    DummyStruct ds2;
    bReader.setBinaryPayload(bWriter.getString());
    bReader.visit(ds2);
    bWriter.clear();
    
    debugPrinter.visit(ds2);
    printf("ds2\n%s\n", debugPrinter.getString().c_str());
    return 0;
}

