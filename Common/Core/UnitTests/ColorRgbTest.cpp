#include "gtest/gtest.h"
#include "Core/Color.h"

using namespace Realisim;
using namespace Core;

//-----------------------------------------------------------------------------
// ColorRgbUint8
//-----------------------------------------------------------------------------
TEST(ColorRgbUint8, Constructor)
{
    ColorRgbUint8 rgb0;
    EXPECT_EQ(rgb0.getRed(), 0);
    EXPECT_EQ(rgb0.getGreen(), 0);
    EXPECT_EQ(rgb0.getBlue(), 0);

    ColorRgbUint8 rgb1(10, 20, 30);
    EXPECT_EQ(rgb1.getRed(), 10);
    EXPECT_EQ(rgb1.getGreen(), 20);
    EXPECT_EQ(rgb1.getBlue(), 30);

    ColorRgbUint8 rgb2(rgb1);
    EXPECT_EQ(rgb2.getRed(), 10);
    EXPECT_EQ(rgb2.getGreen(), 20);
    EXPECT_EQ(rgb2.getBlue(), 30);

    ColorRgbUint8 rgb3;
    rgb3 = rgb1;
    EXPECT_EQ(rgb3.getRed(), 10);
    EXPECT_EQ(rgb3.getGreen(), 20);
    EXPECT_EQ(rgb3.getBlue(), 30);
}

TEST(ColorRgbUint8, operators)
{
    // all gets already tested in Constructor...
    ColorRgbUint8 rgb0(10, 20, 30);
    ColorRgbUint8 rgb1(10, 20, 30);
    ColorRgbUint8 rgb2(11, 20, 30);

    EXPECT_TRUE(rgb0 == rgb1);
    EXPECT_TRUE(rgb0 != rgb2);
}

TEST(ColorRgbUint8, sets)
{
    ColorRgbUint8 rgb;
    rgb.set(10, 20, 30);
    EXPECT_EQ(rgb.getRed(), 10);
    EXPECT_EQ(rgb.getGreen(), 20);
    EXPECT_EQ(rgb.getBlue(), 30);

    rgb.setRed(100);
    EXPECT_EQ(rgb.getRed(), 100);

    rgb.setGreen(200);
    EXPECT_EQ(rgb.getGreen(), 200);

    rgb.setBlue(255);
    EXPECT_EQ(rgb.getBlue(), 255);
}

TEST(ColorRgbUint8, cornerCases)
{
    // out of bound...
    // This actually do not compile with the current warning level!
    //
    /*ColorRgb8 rgb;
    rgb.set(257, 258, 259);
    EXPECT_NE(rgb.getRed(), 257);
    EXPECT_NE(rgb.getGreen(), 258);
    EXPECT_NE(rgb.getBlue(), 259);*/
}

TEST(ColorRgb, operatorTestColorRGB)
{
    ColorRgbInt32 c0(1, 2, 3);
    ColorRgbInt32 c1(11, 12, 13);

    EXPECT_TRUE(c0 == ColorRgbInt32(1, 2, 3));
    EXPECT_TRUE(c1 != ColorRgbInt32(1, 2, 3));

    {
        EXPECT_TRUE(ColorRgbInt32(12, 14, 16) == (c0 + c1));
        ColorRgbInt32 t(c0);
        EXPECT_TRUE(ColorRgbInt32(12, 14, 16) == (t += c1));
    }

    {
        EXPECT_TRUE(ColorRgbInt32(-10, -10, -10) == (c0 - c1));
        ColorRgbInt32 t(c0);
        EXPECT_TRUE(ColorRgbInt32(-10, -10, -10) == (t -= c1));
    }

    {
        EXPECT_TRUE(ColorRgbInt32(11, 24, 39) == (c0 * c1));
        ColorRgbInt32 t(c0);
        EXPECT_TRUE(ColorRgbInt32(11, 24, 39) == (t *= c1));
    }

    {
        EXPECT_TRUE(ColorRgbInt32(1 / 11, 2 / 12, 3 / 13) == (c0 / c1));
        ColorRgbInt32 t(c0);
        EXPECT_TRUE(ColorRgbInt32(1 / 11, 2 / 12, 3 / 13) == (t /= c1));
    }

    {
        EXPECT_TRUE(ColorRgbInt32(2, 4, 6) == (c0 * 2));
        ColorRgbInt32 t(c0);
        EXPECT_TRUE(ColorRgbInt32(2, 4, 6) == (t *= 2));
    }

    {
        EXPECT_TRUE(ColorRgbInt32((int)(1 / 2.0), (int)(2 / 2.0), (int)(3 / 2.0)) == (c0 / 2.0));
        ColorRgbInt32 t(c0);
        EXPECT_TRUE(ColorRgbInt32((int)(1 / 2.0), (int)(2 / 2.0), (int)(3 / 2.0)) == (t /= 2.0));
    }

}

//-----------------------------------------------------------------------------
// ColorRgba
//-----------------------------------------------------------------------------
TEST(ColorRgbaF32, Constructor)
{
    ColorRgbaF32 rgb0;
    EXPECT_EQ(rgb0.getRed(), 0.0f);
    EXPECT_EQ(rgb0.getGreen(), 0.0f);
    EXPECT_EQ(rgb0.getBlue(), 0.0f);
    EXPECT_EQ(rgb0.getAlpha(), 0.0f);

    ColorRgbaF32 rgb1(10, 20, 30, 40);
    EXPECT_EQ(rgb1.getRed(), 10);
    EXPECT_EQ(rgb1.getGreen(), 20);
    EXPECT_EQ(rgb1.getBlue(), 30);
    EXPECT_EQ(rgb1.getAlpha(), 40);

    ColorRgbaF32 rgb2(rgb1);
    EXPECT_EQ(rgb2.getRed(), 10);
    EXPECT_EQ(rgb2.getGreen(), 20);
    EXPECT_EQ(rgb2.getBlue(), 30);
    EXPECT_EQ(rgb2.getAlpha(), 40);

    ColorRgbaF32 rgb3;
    rgb3 = rgb1;
    EXPECT_EQ(rgb3.getRed(), 10);
    EXPECT_EQ(rgb3.getGreen(), 20);
    EXPECT_EQ(rgb3.getBlue(), 30);
    EXPECT_EQ(rgb3.getAlpha(), 40);
}

TEST(ColorRgbaF32, operators)
{
    // all gets already tested in Constructor...
    ColorRgbaF32 rgb0(10, 20, 30, 40);
    ColorRgbaF32 rgb1(10, 20, 30, 40);
    ColorRgbaF32 rgb2(11, 20, 30, 40);

    EXPECT_TRUE(rgb0 == rgb1);
    EXPECT_TRUE(rgb0 != rgb2);
}

TEST(ColorRgbaF32, sets)
{
    ColorRgbaF32 rgb;
    rgb.set(10, 20, 30, 40);
    EXPECT_EQ(rgb.getRed(), 10);
    EXPECT_EQ(rgb.getGreen(), 20);
    EXPECT_EQ(rgb.getBlue(), 30);
    EXPECT_EQ(rgb.getAlpha(), 40);

    rgb.setRed(100);
    EXPECT_EQ(rgb.getRed(), 100);

    rgb.setGreen(200);
    EXPECT_EQ(rgb.getGreen(), 200);

    rgb.setBlue(300);
    EXPECT_EQ(rgb.getBlue(), 300);

    rgb.setAlpha(400);
    EXPECT_EQ(rgb.getAlpha(), 400);
}

TEST(ColorRgba, operatorTestColorRGBA)
{
    ColorRgbaInt32 c0(1, 2, 3, 4);
    ColorRgbaInt32 c1(11, 12, 13, 14);

    EXPECT_TRUE(c0 == ColorRgbaInt32(1, 2, 3, 4));
    EXPECT_TRUE(c1 != ColorRgbaInt32(1, 2, 3, 5));

    {
        EXPECT_TRUE(ColorRgbaInt32(12, 14, 16, 18) == (c0 + c1));
        ColorRgbaInt32 t(c0);
        EXPECT_TRUE(ColorRgbaInt32(12, 14, 16, 18) == (t += c1));
    }

    {
        EXPECT_TRUE(ColorRgbaInt32(-10, -10, -10, -10) == (c0 - c1));
        ColorRgbaInt32 t(c0);
        EXPECT_TRUE(ColorRgbaInt32(-10, -10, -10, -10) == (t -= c1));
    }

    {
        EXPECT_TRUE(ColorRgbaInt32(11, 24, 39, 56) == (c0 * c1));
        ColorRgbaInt32 t(c0);
        EXPECT_TRUE(ColorRgbaInt32(11, 24, 39, 56) == (t *= c1));
    }

    {
        EXPECT_TRUE(ColorRgbaInt32(1 / 11, 2 / 12, 3 / 13, 4 / 14) == (c0 / c1));
        ColorRgbaInt32 t(c0);
        EXPECT_TRUE(ColorRgbaInt32(1 / 11, 2 / 12, 3 / 13, 4 / 14) == (t /= c1));
    }

    {
        EXPECT_TRUE(ColorRgbaInt32(2, 4, 6, 8) == (c0 * 2));
        ColorRgbaInt32 t(c0);
        EXPECT_TRUE(ColorRgbaInt32(2, 4, 6, 8) == (t *= 2));
    }

    {
        EXPECT_TRUE(ColorRgbaInt32((int)(1 / 2.0), (int)(2 / 2.0), (int)(3 / 2.0), (int)(4 / 2.0)) == (c0 / 2.0));
        ColorRgbaInt32 t(c0);
        EXPECT_TRUE(ColorRgbaInt32((int)(1 / 2.0), (int)(2 / 2.0), (int)(3 / 2.0), (int)(4 / 2.0)) == (t /= 2.0));
    }

}