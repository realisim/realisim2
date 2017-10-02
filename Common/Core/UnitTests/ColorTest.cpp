#include "gtest/gtest.h"
#include "Core/Color.h"

using namespace Realisim;
using namespace Core;

TEST(Color, int8Test)
{
    Color c0;
    EXPECT_EQ(c0.getRed(), 0.0);
    EXPECT_EQ(c0.getGreen(), 0.0);
    EXPECT_EQ(c0.getBlue(), 0.0);

    // uint8
    {
        const uint8_t r = std::numeric_limits<uint8_t>::min();
        const uint8_t g = std::numeric_limits<uint8_t>::min() + 1;
        const uint8_t b = std::numeric_limits<uint8_t>::max() - 1;
        const uint8_t a = std::numeric_limits<uint8_t>::max();
        Color c1(r, g, b, a);

        EXPECT_EQ(c1.getRedUint8(), r);
        EXPECT_EQ(c1.getGreenUint8(), g);
        EXPECT_EQ(c1.getBlueUint8(), b);
        EXPECT_EQ(c1.getAlphaUint8(), a);

        ColorRgbUint8 rgb = c1.getRgbUint8();
        EXPECT_EQ(rgb.getRed(), r);
        EXPECT_EQ(rgb.getGreen(), g);
        EXPECT_EQ(rgb.getBlue(), b);

        ColorRgbaUint8 rgba = c1.getRgbaUint8();
        EXPECT_EQ(rgba.getRed(), r);
        EXPECT_EQ(rgba.getGreen(), g);
        EXPECT_EQ(rgba.getBlue(), b);
        EXPECT_EQ(rgba.getAlpha(), a);

        Color c2;
        c2.setRgb(rgb);
        EXPECT_EQ(c2.getRedUint8(), r);
        EXPECT_EQ(c2.getGreenUint8(), g);
        EXPECT_EQ(c2.getBlueUint8(), b);        

        Color c3;
        c3.setRgba(rgba);
        EXPECT_EQ(c3.getRedUint8(), r);
        EXPECT_EQ(c3.getGreenUint8(), g);
        EXPECT_EQ(c3.getBlueUint8(), b);
        EXPECT_EQ(c3.getAlphaUint8(), a);

        c3.setRedUint8(r);
        c3.setGreenUint8(g);
        c3.setBlueUint8(b);
        c3.setAlphaUint8(a);

        EXPECT_EQ(c3.getRedUint8(), r);
        EXPECT_EQ(c3.getGreenUint8(), g);
        EXPECT_EQ(c3.getBlueUint8(), b);
        EXPECT_EQ(c3.getAlphaUint8(), a);
    }

    // int8
    {
        const int8_t r = std::numeric_limits<int8_t>::min();
        const int8_t g = std::numeric_limits<int8_t>::min() + 1;
        const int8_t b = std::numeric_limits<int8_t>::max() - 1;
        const int8_t a = std::numeric_limits<int8_t>::max();
        Color c1(r, g, b, a);

        EXPECT_EQ(c1.getRedInt8(), r);
        EXPECT_EQ(c1.getGreenInt8(), g);
        EXPECT_EQ(c1.getBlueInt8(), b);
        EXPECT_EQ(c1.getAlphaInt8(), a);

        ColorRgbInt8 rgb = c1.getRgbInt8();
        EXPECT_EQ(rgb.getRed(), r);
        EXPECT_EQ(rgb.getGreen(), g);
        EXPECT_EQ(rgb.getBlue(), b);

        ColorRgbaInt8 rgba = c1.getRgbaInt8();
        EXPECT_EQ(rgba.getRed(), r);
        EXPECT_EQ(rgba.getGreen(), g);
        EXPECT_EQ(rgba.getBlue(), b);
        EXPECT_EQ(rgba.getAlpha(), a);

        Color c2;
        c2.setRgb(rgb);
        EXPECT_EQ(c2.getRedInt8(), r);
        EXPECT_EQ(c2.getGreenInt8(), g);
        EXPECT_EQ(c2.getBlueInt8(), b);

        Color c3;
        c3.setRgba(rgba);
        EXPECT_EQ(c3.getRedInt8(), r);
        EXPECT_EQ(c3.getGreenInt8(), g);
        EXPECT_EQ(c3.getBlueInt8(), b);
        EXPECT_EQ(c3.getAlphaInt8(), a);

        c3.setRedInt8(r);
        c3.setGreenInt8(g);
        c3.setBlueInt8(b);
        c3.setAlphaInt8(a);

        EXPECT_EQ(c3.getRedInt8(), r);
        EXPECT_EQ(c3.getGreenInt8(), g);
        EXPECT_EQ(c3.getBlueInt8(), b);
        EXPECT_EQ(c3.getAlphaInt8(), a);
    }
}

TEST(Color, int16Test)
{
    // uint16
    {
        const uint16_t r = std::numeric_limits<uint16_t>::min();
        const uint16_t g = std::numeric_limits<uint16_t>::min() + 1;
        const uint16_t b = std::numeric_limits<uint16_t>::max() - 1;
        const uint16_t a = std::numeric_limits<uint16_t>::max();
        Color c1(r, g, b, a);

        EXPECT_EQ(c1.getRedUint16(), r);
        EXPECT_EQ(c1.getGreenUint16(), g);
        EXPECT_EQ(c1.getBlueUint16(), b);
        EXPECT_EQ(c1.getAlphaUint16(), a);

        ColorRgbUint16 rgb = c1.getRgbUint16();
        EXPECT_EQ(rgb.getRed(), r);
        EXPECT_EQ(rgb.getGreen(), g);
        EXPECT_EQ(rgb.getBlue(), b);

        ColorRgbaUint16 rgba = c1.getRgbaUint16();
        EXPECT_EQ(rgba.getRed(), r);
        EXPECT_EQ(rgba.getGreen(), g);
        EXPECT_EQ(rgba.getBlue(), b);
        EXPECT_EQ(rgba.getAlpha(), a);

        Color c2;
        c2.setRgb(rgb);
        EXPECT_EQ(c2.getRedUint16(), r);
        EXPECT_EQ(c2.getGreenUint16(), g);
        EXPECT_EQ(c2.getBlueUint16(), b);

        Color c3;
        c3.setRgba(rgba);
        EXPECT_EQ(c3.getRedUint16(), r);
        EXPECT_EQ(c3.getGreenUint16(), g);
        EXPECT_EQ(c3.getBlueUint16(), b);
        EXPECT_EQ(c3.getAlphaUint16(), a);

        c3.setRedUint16(r);
        c3.setGreenUint16(g);
        c3.setBlueUint16(b);
        c3.setAlphaUint16(a);

        EXPECT_EQ(c3.getRedUint16(), r);
        EXPECT_EQ(c3.getGreenUint16(), g);
        EXPECT_EQ(c3.getBlueUint16(), b);
        EXPECT_EQ(c3.getAlphaUint16(), a);
    }

    // int16
    {
        const int16_t r = std::numeric_limits<int16_t>::min();
        const int16_t g = std::numeric_limits<int16_t>::min() + 1;
        const int16_t b = std::numeric_limits<int16_t>::max() - 1;
        const int16_t a = std::numeric_limits<int16_t>::max();
        Color c1(r, g, b, a);

        EXPECT_EQ(c1.getRedInt16(), r);
        EXPECT_EQ(c1.getGreenInt16(), g);
        EXPECT_EQ(c1.getBlueInt16(), b);
        EXPECT_EQ(c1.getAlphaInt16(), a);

        ColorRgbInt16 rgb = c1.getRgbInt16();
        EXPECT_EQ(rgb.getRed(), r);
        EXPECT_EQ(rgb.getGreen(), g);
        EXPECT_EQ(rgb.getBlue(), b);

        ColorRgbaInt16 rgba = c1.getRgbaInt16();
        EXPECT_EQ(rgba.getRed(), r);
        EXPECT_EQ(rgba.getGreen(), g);
        EXPECT_EQ(rgba.getBlue(), b);
        EXPECT_EQ(rgba.getAlpha(), a);

        Color c2;
        c2.setRgb(rgb);
        EXPECT_EQ(c2.getRedInt16(), r);
        EXPECT_EQ(c2.getGreenInt16(), g);
        EXPECT_EQ(c2.getBlueInt16(), b);

        Color c3;
        c3.setRgba(rgba);
        EXPECT_EQ(c3.getRedInt16(), r);
        EXPECT_EQ(c3.getGreenInt16(), g);
        EXPECT_EQ(c3.getBlueInt16(), b);
        EXPECT_EQ(c3.getAlphaInt16(), a);

        c3.setRedInt16(r);
        c3.setGreenInt16(g);
        c3.setBlueInt16(b);
        c3.setAlphaInt16(a);

        EXPECT_EQ(c3.getRedInt16(), r);
        EXPECT_EQ(c3.getGreenInt16(), g);
        EXPECT_EQ(c3.getBlueInt16(), b);
        EXPECT_EQ(c3.getAlphaInt16(), a);
    }
}

TEST(Color, int32Test)
{
    // uint32
    {
        const uint32_t r = std::numeric_limits<uint32_t>::min();
        const uint32_t g = std::numeric_limits<uint32_t>::min() + 1;
        const uint32_t b = std::numeric_limits<uint32_t>::max() - 1;
        const uint32_t a = std::numeric_limits<uint32_t>::max();
        Color c1(r, g, b, a);

        EXPECT_EQ(c1.getRedUint32(), r);
        EXPECT_EQ(c1.getGreenUint32(), g);
        EXPECT_EQ(c1.getBlueUint32(), b);
        EXPECT_EQ(c1.getAlphaUint32(), a);

        ColorRgbUint32 rgb = c1.getRgbUint32();
        EXPECT_EQ(rgb.getRed(), r);
        EXPECT_EQ(rgb.getGreen(), g);
        EXPECT_EQ(rgb.getBlue(), b);

        ColorRgbaUint32 rgba = c1.getRgbaUint32();
        EXPECT_EQ(rgba.getRed(), r);
        EXPECT_EQ(rgba.getGreen(), g);
        EXPECT_EQ(rgba.getBlue(), b);
        EXPECT_EQ(rgba.getAlpha(), a);

        Color c2;
        c2.setRgb(rgb);
        EXPECT_EQ(c2.getRedUint32(), r);
        EXPECT_EQ(c2.getGreenUint32(), g);
        EXPECT_EQ(c2.getBlueUint32(), b);

        Color c3;
        c3.setRgba(rgba);
        EXPECT_EQ(c3.getRedUint32(), r);
        EXPECT_EQ(c3.getGreenUint32(), g);
        EXPECT_EQ(c3.getBlueUint32(), b);
        EXPECT_EQ(c3.getAlphaUint32(), a);

        c3.setRedUint32(r);
        c3.setGreenUint32(g);
        c3.setBlueUint32(b);
        c3.setAlphaUint32(a);

        EXPECT_EQ(c3.getRedUint32(), r);
        EXPECT_EQ(c3.getGreenUint32(), g);
        EXPECT_EQ(c3.getBlueUint32(), b);
        EXPECT_EQ(c3.getAlphaUint32(), a);
    }

    // int32
    {
        const int32_t r = std::numeric_limits<int32_t>::min();
        const int32_t g = std::numeric_limits<int32_t>::min() + 1;
        const int32_t b = std::numeric_limits<int32_t>::max() - 1;
        const int32_t a = std::numeric_limits<int32_t>::max();
        Color c1(r, g, b, a);

        EXPECT_EQ(c1.getRedInt32(), r);
        EXPECT_EQ(c1.getGreenInt32(), g);
        EXPECT_EQ(c1.getBlueInt32(), b);
        EXPECT_EQ(c1.getAlphaInt32(), a);

        ColorRgbInt32 rgb = c1.getRgbInt32();
        EXPECT_EQ(rgb.getRed(), r);
        EXPECT_EQ(rgb.getGreen(), g);
        EXPECT_EQ(rgb.getBlue(), b);

        ColorRgbaInt32 rgba = c1.getRgbaInt32();
        EXPECT_EQ(rgba.getRed(), r);
        EXPECT_EQ(rgba.getGreen(), g);
        EXPECT_EQ(rgba.getBlue(), b);
        EXPECT_EQ(rgba.getAlpha(), a);

        Color c2;
        c2.setRgb(rgb);
        EXPECT_EQ(c2.getRedInt32(), r);
        EXPECT_EQ(c2.getGreenInt32(), g);
        EXPECT_EQ(c2.getBlueInt32(), b);

        Color c3;
        c3.setRgba(rgba);
        EXPECT_EQ(c3.getRedInt32(), r);
        EXPECT_EQ(c3.getGreenInt32(), g);
        EXPECT_EQ(c3.getBlueInt32(), b);
        EXPECT_EQ(c3.getAlphaInt32(), a);

        c3.setRedInt32(r);
        c3.setGreenInt32(g);
        c3.setBlueInt32(b);
        c3.setAlphaInt32(a);

        EXPECT_EQ(c3.getRedInt32(), r);
        EXPECT_EQ(c3.getGreenInt32(), g);
        EXPECT_EQ(c3.getBlueInt32(), b);
        EXPECT_EQ(c3.getAlphaInt32(), a);
    }
}

TEST(Color, halffloatTest)
{
    using namespace half_float;
    using namespace half_float::literal;

    // float
    {
        const half r = std::numeric_limits<half>::min();
        const half g = std::numeric_limits<half>::min() + 1.0_h;
        const half b = std::numeric_limits<half>::max() - 1.0_h;
        const half a = std::numeric_limits<half>::max();
        Color c1(r, g, b, a);

        // we cannot use EXPECT_EQ with half float due to the streaming operator
        // in gtest...

        EXPECT_TRUE(c1.getRedF16() == r);
        EXPECT_TRUE(c1.getGreenF16() == g);
        EXPECT_TRUE(c1.getBlueF16() == b);
        EXPECT_TRUE(c1.getAlphaF16() == a);

        ColorRgbF16 rgb = c1.getRgbF16();
        EXPECT_TRUE(rgb.getRed() == r);
        EXPECT_TRUE(rgb.getGreen() == g);
        EXPECT_TRUE(rgb.getBlue() == b);

        ColorRgbaF16 rgba = c1.getRgbaF16();
        EXPECT_TRUE(rgba.getRed() == r);
        EXPECT_TRUE(rgba.getGreen() == g);
        EXPECT_TRUE(rgba.getBlue() == b);
        EXPECT_TRUE(rgba.getAlpha() == a);

        Color c2;
        c2.setRgb(rgb);
        EXPECT_TRUE(c2.getRedF16() == r);
        EXPECT_TRUE(c2.getGreenF16() == g);
        EXPECT_TRUE(c2.getBlueF16() == b);

        Color c3;
        c3.setRgba(rgba);
        EXPECT_TRUE(c3.getRedF16() == r);
        EXPECT_TRUE(c3.getGreenF16() == g);
        EXPECT_TRUE(c3.getBlueF16() == b);
        EXPECT_TRUE(c3.getAlphaF16() == a);

        c3.setRedF16(r);
        c3.setGreenF16(g);
        c3.setBlueF16(b);
        c3.setAlphaF16(a);

        EXPECT_TRUE(c3.getRedF16() == r);
        EXPECT_TRUE(c3.getGreenF16() == g);
        EXPECT_TRUE(c3.getBlueF16() == b);
        EXPECT_TRUE(c3.getAlphaF16() == a);
    }

    // half
    {
        const half r = -std::numeric_limits<half>::max();
        const half g = (half)0.233f;
        const half b = (half)1878.8734f;
        const half a = std::numeric_limits<half>::max();
        Color c1(r, g, b, a);

        EXPECT_TRUE(c1.getRedF16() == r);
        EXPECT_TRUE(c1.getGreenF16() == g);
        EXPECT_TRUE(c1.getBlueF16() == b);
        EXPECT_TRUE(c1.getAlphaF16() == a);

        ColorRgbF16 rgb = c1.getRgbF16();
        EXPECT_TRUE(rgb.getRed() == r);
        EXPECT_TRUE(rgb.getGreen() == g);
        EXPECT_TRUE(rgb.getBlue() == b);

        ColorRgbaF16 rgba = c1.getRgbaF16();
        EXPECT_TRUE(rgba.getRed() == r);
        EXPECT_TRUE(rgba.getGreen() == g);
        EXPECT_TRUE(rgba.getBlue() == b);
        EXPECT_TRUE(rgba.getAlpha() == a);

        Color c2;
        c2.setRgb(rgb);
        EXPECT_TRUE(c2.getRedF16() == r);
        EXPECT_TRUE(c2.getGreenF16() == g);
        EXPECT_TRUE(c2.getBlueF16() == b);

        Color c3;
        c3.setRgba(rgba);
        EXPECT_TRUE(c3.getRedF16() == r);
        EXPECT_TRUE(c3.getGreenF16() == g);
        EXPECT_TRUE(c3.getBlueF16() == b);
        EXPECT_TRUE(c3.getAlphaF16() == a);

        c3.setRedF16(r);
        c3.setGreenF16(g);
        c3.setBlueF16(b);
        c3.setAlphaF16(a);

        EXPECT_TRUE(c3.getRedF16() == r);
        EXPECT_TRUE(c3.getGreenF16() == g);
        EXPECT_TRUE(c3.getBlueF16() == b);
        EXPECT_TRUE(c3.getAlphaF16() == a);
    }
}

TEST(Color, floatTest)
{
    // float
    {
        const float r = std::numeric_limits<float>::min();
        const float g = std::numeric_limits<float>::min() + 1;
        const float b = std::numeric_limits<float>::max() - 1;
        const float a = std::numeric_limits<float>::max();
        Color c1(r, g, b, a);

        EXPECT_EQ(c1.getRedF32(), r);
        EXPECT_EQ(c1.getGreenF32(), g);
        EXPECT_EQ(c1.getBlueF32(), b);
        EXPECT_EQ(c1.getAlphaF32(), a);

        ColorRgbF32 rgb = c1.getRgbF32();
        EXPECT_EQ(rgb.getRed(), r);
        EXPECT_EQ(rgb.getGreen(), g);
        EXPECT_EQ(rgb.getBlue(), b);

        ColorRgbaF32 rgba = c1.getRgbaF32();
        EXPECT_EQ(rgba.getRed(), r);
        EXPECT_EQ(rgba.getGreen(), g);
        EXPECT_EQ(rgba.getBlue(), b);
        EXPECT_EQ(rgba.getAlpha(), a);

        Color c2;
        c2.setRgb(rgb);
        EXPECT_EQ(c2.getRedF32(), r);
        EXPECT_EQ(c2.getGreenF32(), g);
        EXPECT_EQ(c2.getBlueF32(), b);

        Color c3;
        c3.setRgba(rgba);
        EXPECT_EQ(c3.getRedF32(), r);
        EXPECT_EQ(c3.getGreenF32(), g);
        EXPECT_EQ(c3.getBlueF32(), b);
        EXPECT_EQ(c3.getAlphaF32(), a);

        c3.setRedF32(r);
        c3.setGreenF32(g);
        c3.setBlueF32(b);
        c3.setAlphaF32(a);

        EXPECT_EQ(c3.getRedF32(), r);
        EXPECT_EQ(c3.getGreenF32(), g);
        EXPECT_EQ(c3.getBlueF32(), b);
        EXPECT_EQ(c3.getAlphaF32(), a);
    }

    // float
    {
        const float r = -std::numeric_limits<float>::max();
        const float g = 0.233f;
        const float b = 1878.8734f;
        const float a = std::numeric_limits<float>::max();
        Color c1(r, g, b, a);

        EXPECT_EQ(c1.getRedF32(), r);
        EXPECT_EQ(c1.getGreenF32(), g);
        EXPECT_EQ(c1.getBlueF32(), b);
        EXPECT_EQ(c1.getAlphaF32(), a);

        ColorRgbF32 rgb = c1.getRgbF32();
        EXPECT_EQ(rgb.getRed(), r);
        EXPECT_EQ(rgb.getGreen(), g);
        EXPECT_EQ(rgb.getBlue(), b);

        ColorRgbaF32 rgba = c1.getRgbaF32();
        EXPECT_EQ(rgba.getRed(), r);
        EXPECT_EQ(rgba.getGreen(), g);
        EXPECT_EQ(rgba.getBlue(), b);
        EXPECT_EQ(rgba.getAlpha(), a);

        Color c2;
        c2.setRgb(rgb);
        EXPECT_EQ(c2.getRedF32(), r);
        EXPECT_EQ(c2.getGreenF32(), g);
        EXPECT_EQ(c2.getBlueF32(), b);

        Color c3;
        c3.setRgba(rgba);
        EXPECT_EQ(c3.getRedF32(), r);
        EXPECT_EQ(c3.getGreenF32(), g);
        EXPECT_EQ(c3.getBlueF32(), b);
        EXPECT_EQ(c3.getAlphaF32(), a);

        c3.setRedF32(r);
        c3.setGreenF32(g);
        c3.setBlueF32(b);
        c3.setAlphaF32(a);

        EXPECT_EQ(c3.getRedF32(), r);
        EXPECT_EQ(c3.getGreenF32(), g);
        EXPECT_EQ(c3.getBlueF32(), b);
        EXPECT_EQ(c3.getAlphaF32(), a);
    }
}