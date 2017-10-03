#include "gtest/gtest.h"
#include "Core/StreamUtility.h"
#include <stdint.h>
#include <sstream>

using namespace Realisim;
    using namespace Core;
using namespace std;

TEST(StreamUtiliy, writeInLittleEndian)
{
    StreamUtility su;
    su.setStreamFormat(StreamUtility::eLittleEndian);

    const int8_t int8 = 0x0f;
    const int16_t int16 = 0x0fff;
    const int32_t int32 = 0x0fffffff;
    const int64_t int64 = 0x0fffffffFfffffff;
    const uint8_t uint8 = 0xff;
    const uint16_t uint16 = 0xffff;
    const uint32_t uint32 = 0xffffffff;
    const uint64_t uint64 = 0xffffffffffffffff;
    const float aFloat = 155.0f;
    const double aDouble= 155.55f;    
    std::string asciiText("the quick brown fox jumps over the lazy dog.");
    std::string nonAsciiText("This is an example of non ascii text!@$#%?&*()éçè{øñþìcÈ1234");
    const std::string referenceData = { '\x0', '\x1', '\x0', '\x1',
        '\x0', '\x1', '\x0', '\x1',
        '\x0', '\x1', '\x0', '\x1' };
    ByteArray baBinary(referenceData);


    ostringstream oss;
    su.write(oss, int8);
    su.write(oss, int16);
    su.write(oss, int32);
    su.write(oss, int64);

    su.write(oss, uint8);
    su.write(oss, uint16);
    su.write(oss, uint32);
    su.write(oss, uint64);

    su.write(oss, aFloat);
    su.write(oss, aDouble);

    su.writeChar(oss, asciiText);
    // non ascii text needs to be saved as bytes since it might contains \0
    su.writeBytes(oss, nonAsciiText);
    su.write(oss, baBinary);

    //check values.
    istringstream iss(oss.str());
    int8_t cInt8;
    int16_t cInt16;
    int32_t cInt32;
    int64_t cInt64;

    uint8_t cUInt8;
    uint16_t cUInt16;
    uint32_t cUInt32;
    uint64_t cUInt64;

    float cFloat;
    double cDouble;

    string cAsciiText, cNonAsciiText, binaryData;

    su.readInt8(iss, &cInt8);
    su.readInt16(iss, &cInt16);
    su.readInt32(iss, &cInt32);
    su.readInt64(iss, &cInt64);
    su.readUint8(iss, &cUInt8);
    su.readUint16(iss, &cUInt16);
    su.readUint32(iss, &cUInt32);
    su.readUint64(iss, &cUInt64);
    su.readFloat32(iss, &cFloat);
    su.readDouble(iss, &cDouble);
    su.readChar(iss, (int)asciiText.size()+1, &cAsciiText); //+1 to account for the \0 written by writeChar
    //non ascii text must be read as bytes...
    su.readBytes(iss, (int)nonAsciiText.size(), &cNonAsciiText);
    su.readBytes(iss, 12, &binaryData);

    EXPECT_EQ(int8, cInt8);
    EXPECT_EQ(int16, cInt16);
    EXPECT_EQ(int32, cInt32);
    EXPECT_EQ(int64, cInt64);
    EXPECT_EQ(uint8, cUInt8);
    EXPECT_EQ(uint16, cUInt16);
    EXPECT_EQ(uint32, cUInt32);
    EXPECT_EQ(uint64, cUInt64);
    EXPECT_EQ(aFloat, cFloat);
    EXPECT_EQ(aDouble, cDouble);
    EXPECT_STREQ(asciiText.c_str(), asciiText.c_str());
    EXPECT_STREQ(nonAsciiText.c_str(), cNonAsciiText.c_str());

    EXPECT_EQ(binaryData.size(), referenceData.size());
    for (int i = 0; i < referenceData.size(); ++i)
    {
        EXPECT_EQ(binaryData[i], referenceData[i]);
    }
}

//same test as above but writting in big endian...
TEST(StreamUtiliy, writeInBigEndian)
{
    StreamUtility su;
    su.setStreamFormat(StreamUtility::eBigEndian);

    const int8_t int8 = 0x0f;
    const int16_t int16 = 0x0fff;
    const int32_t int32 = 0x0fffffff;
    const int64_t int64 = 0x0fffffffffffffff;
    const uint8_t uint8 = 0xff;
    const uint16_t uint16 = 0xffff;
    const uint32_t uint32 = 0xffffffff;
    const uint64_t uint64 = 0xffffffffffffffff;
    const float aFloat = 155.0f;
    const double aDouble = 155.55f;
    std::string asciiText("the quick brown fox jumps over the lazy dog.");
    std::string nonAsciiText("This is an example of non ascii text!@$#%?&*()éçè{øñþìcÈ1234");
    const std::string referenceData = { '\x0', '\x1', '\x0', '\x1',
        '\x0', '\x1', '\x0', '\x1',
        '\x0', '\x1', '\x0', '\x1' };
    ByteArray baBinary(referenceData);


    ostringstream oss;
    su.write(oss, int8);
    su.write(oss, int16);
    su.write(oss, int32);
    su.write(oss, int64);

    su.write(oss, uint8);
    su.write(oss, uint16);
    su.write(oss, uint32);
    su.write(oss, uint64);

    su.write(oss, aFloat);
    su.write(oss, aDouble);

    su.writeChar(oss, asciiText);
    // non ascii text needs to be saved as bytes since it might contains \0
    su.writeBytes(oss, nonAsciiText);
    su.write(oss, baBinary);

    //check values.
    istringstream iss(oss.str());
    int8_t cInt8;
    int16_t cInt16;
    int32_t cInt32;
    int64_t cInt64;

    uint8_t cUInt8;
    uint16_t cUInt16;
    uint32_t cUInt32;
    uint64_t cUInt64;

    float cFloat;
    double cDouble;

    string cAsciiText, cNonAsciiText, binaryData;

    su.readInt8(iss, &cInt8);
    su.readInt16(iss, &cInt16);
    su.readInt32(iss, &cInt32);
    su.readInt64(iss, &cInt64);
    su.readUint8(iss, &cUInt8);
    su.readUint16(iss, &cUInt16);
    su.readUint32(iss, &cUInt32);
    su.readUint64(iss, &cUInt64);
    su.readFloat32(iss, &cFloat);
    su.readDouble(iss, &cDouble);
    su.readChar(iss, (int)asciiText.size() + 1, &cAsciiText); //+1 to account for the \0 written by writeChar
                                                              //non ascii text must be read as bytes...
    su.readBytes(iss, (int)nonAsciiText.size(), &cNonAsciiText);
    su.readBytes(iss, 12, &binaryData);

    EXPECT_EQ(int8, cInt8);
    EXPECT_EQ(int16, cInt16);
    EXPECT_EQ(int32, cInt32);
    EXPECT_EQ(int64, cInt64);
    EXPECT_EQ(uint8, cUInt8);
    EXPECT_EQ(uint16, cUInt16);
    EXPECT_EQ(uint32, cUInt32);
    EXPECT_EQ(uint64, cUInt64);
    EXPECT_EQ(aFloat, cFloat);
    EXPECT_EQ(aDouble, cDouble);
    EXPECT_STREQ(asciiText.c_str(), asciiText.c_str());
    EXPECT_STREQ(nonAsciiText.c_str(), cNonAsciiText.c_str());

    EXPECT_EQ(binaryData.size(), referenceData.size());
    for (int i = 0; i < referenceData.size(); ++i)
    {
        EXPECT_EQ(binaryData[i], referenceData[i]);
    }
}

