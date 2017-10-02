#include "gtest/gtest.h"
#include "Core/Image.h"
#include "Core/ImageSupport/ImageInternalFormat.h"
#include "Core/Path.h"
#include "Core/FileInfo.h"
#include "Core/Timer.h"

using namespace Realisim;
    using namespace Core;
    using namespace Math;

namespace
{
    std::string getCurrentFolderPath()
    {
        FileInfo fi(Path::getApplicationFilePath());
        return fi.getCanonicalPath();
    }

    std::string getAssetsPath()
    {
        // return path to executable
        FileInfo fi(Path::getApplicationFilePath());
        return fi.getCanonicalPath() + "/../CoreAssets";
    }
}

TEST(Image, saveAndLoad)
{
    Image source;

    {
        int w = 4;
        int h = 5;
        const unsigned char v = 255;
        ImageInternalFormat iif = iifRgbUint8;
        const int numChannels = getNumberOfChannels(iif);
        const int bytesPerChannel = getBytesPerChannel(iif);
        ByteArray sourceData(std::string(w * h * bytesPerChannel * numChannels, v));

        source.setData(w, h, iif, sourceData.constData());
        source.saveAs(getCurrentFolderPath() + "/saveAndLoad.png", Image::wfPng);

        source.load();
        EXPECT_EQ(iif, source.getInternalFormat());
    }
    
    {
        int w = 4;
        int h = 5;
        const unsigned char v = 255;
        ImageInternalFormat iif = iifRgbaUint8;
        const int numChannels = getNumberOfChannels(iif);
        const int bytesPerChannel = getBytesPerChannel(iif);
        ByteArray sourceData(std::string(w * h * bytesPerChannel * numChannels, v));

        source.setData(w, h, iif, sourceData.constData());
        source.saveAs(getCurrentFolderPath() + "/saveAndLoad.png", Image::wfPng);

        source.load();
        EXPECT_EQ(iif, source.getInternalFormat());
    }
    
}

//-----------------------------------------------------------------------------
// 8 bits images
//-----------------------------------------------------------------------------
TEST(Image, get_set_PixelColor_no_interpolation_8_bits)
{
    Image source;
    Image destination;

    int w = 4;
    int h = 4;
    const unsigned char v = 255;
    ByteArray sourceData(std::string(w * h * 3, v));

    //create a 4x4 white image
    source.setData(w, h, iifRgbUint8, sourceData.constData());
    source.saveAs(getCurrentFolderPath() + "/white4x4.png", Image::wfPng);

    //modify tge image so the first 2x2 pixels have a different color
    source.setPixelColor(Math::Vector2i(0, 0), Color(1.0, 0.0, 0.0));
    source.setPixelColor(Math::Vector2i(1, 0), Color(0.0, 1.0, 0.0));
    source.setPixelColor(Math::Vector2i(1, 1), Color(0.0, 0.0, 1.0));
    source.setPixelColor(Math::Vector2i(0, 1), Color(1.0, 1.0, 0.0));
    source.saveAs(getCurrentFolderPath() + "/coloredCorners.png", Image::wfPng);

    //get the pixel value
    Color r = source.getPixelColor(Vector2i(0, 0));
    Color g = source.getPixelColor(Vector2i(1, 0));
    Color b = source.getPixelColor(Vector2i(1, 1));
    Color yellow = source.getPixelColor(Vector2i(0, 1));
    EXPECT_TRUE(r == Color(1.0, 0.0, 0.0));
    EXPECT_TRUE(g == Color(0.0, 1.0, 0.0));
    EXPECT_TRUE(b == Color(0.0, 0.0, 1.0));
    EXPECT_TRUE(yellow == Color(1.0, 1.0, 0.0));
}

TEST(Image, get_set_PixelColor_with_Interpolation_8_bits)
{
    Image source;
    Image destination;

    int w = 3;
    int h = 3;
    const unsigned char v = 255;
    ByteArray sourceData(std::string(w * h * 3, v));

    //create a 3x3 white image
    source.setData(w, h, iifRgbUint8, sourceData.constData());
    //source.save(getCurrentFolderPath() + "/white4x4.png", Image::wfPng);

    // modify tge image so we have black, red, green and blue in each
    // corner and a bit of interpolation in between

    source.setPixelColor(Math::Vector2i(0, 0), Color(0.0, 0.0, 0.0));
    source.setPixelColor(Math::Vector2i(1, 0), Color(127/255.0, 0.0, 0.0));
    source.setPixelColor(Math::Vector2i(2, 0), Color(1.0, 0.0, 0.0));

    source.setPixelColor(Math::Vector2i(0, 1), Color(0.0, 0.0, 127 / 255.0));
    source.setPixelColor(Math::Vector2i(1, 1), Color(1.0, 1.0, 1.0));
    source.setPixelColor(Math::Vector2i(2, 1), Color(0.5, 0.5, 0.0));

    source.setPixelColor(Math::Vector2i(0, 2), Color(0.0, 0.0, 1.0));
    source.setPixelColor(Math::Vector2i(1, 2), Color(0.0, 127 / 255.0, 127 / 255.0));
    source.setPixelColor(Math::Vector2i(2, 2), Color(0.0, 1.0, 0.0));
    source.saveAs(getCurrentFolderPath() + "/coloredCorners2.png", Image::wfPng);

    //get the pixel value with interpolated colors
    Color center = source.getPixelColor(Vector2(1.5, 1.5));
    Color bl = source.getPixelColor(Vector2(1.25, 1.25));
    Color br = source.getPixelColor(Vector2(1.75, 1.25));
    Color tr = source.getPixelColor(Vector2(1.75, 1.75));
    Color tl = source.getPixelColor(Vector2(1.25, 1.75));

    Color t = source.getPixelColor(Vector2(1.5, 2.0));
    Color b = source.getPixelColor(Vector2(1.5, 1.0));
    Color l = source.getPixelColor(Vector2(1.0, 1.5));
    Color r = source.getPixelColor(Vector2(2.0, 1.5));
        
    // test center
    EXPECT_TRUE(center == Color(1.0, 1.0, 1.0));

    // test 4 positions around center
    EXPECT_TRUE(bl.getRgbUint8() == ColorRgbUint8(167, 143, 167));
    EXPECT_TRUE(br.getRgbUint8() == ColorRgbUint8(207, 167, 143));
    EXPECT_TRUE(tr.getRgbUint8() == ColorRgbUint8(167, 207, 167));
    EXPECT_TRUE(tl.getRgbUint8() == ColorRgbUint8(143, 167, 207));

    // test position directly on pixel edges
    EXPECT_TRUE(t.getRgbUint8() == ColorRgbUint8(127, 191, 191));
    EXPECT_TRUE(b.getRgbUint8() == ColorRgbUint8(191, 127, 127));
    EXPECT_TRUE(l.getRgbUint8() == ColorRgbUint8(127, 127, 191));
    EXPECT_TRUE(r.getRgbUint8() == ColorRgbUint8(191, 191, 127));
}

//-----------------------------------------------------------------------------
// 16 bits images
//-----------------------------------------------------------------------------
TEST(Image, get_set_PixelColor_no_interpolation_16_bits)
{
    Image source;
    Image destination;

    int w = 4;
    int h = 4;
    const unsigned short v = 255;
    ByteArray sourceData(std::string(w * h * 3 * 2, v));

    //create a 4x4 white image
    source.setData(w, h, iifRgbUint16, sourceData.constData());
    source.saveAs(getCurrentFolderPath() + "/white_16_4x4.png", Image::wfPng);

    //modify tge image so the first 2x2 pixels have a different color
    source.setPixelColor(Math::Vector2i(0, 0), Color(1.0, 0.0, 0.0));
    source.setPixelColor(Math::Vector2i(1, 0), Color(0.0, 1.0, 0.0));
    source.setPixelColor(Math::Vector2i(1, 1), Color(0.0, 0.0, 1.0));
    source.setPixelColor(Math::Vector2i(0, 1), Color(1.0, 1.0, 0.0));
    source.saveAs(getCurrentFolderPath() + "/coloredCorners_16.png", Image::wfPng);

    //get the pixel value
    Color r = source.getPixelColor(Vector2i(0, 0));
    Color g = source.getPixelColor(Vector2i(1, 0));
    Color b = source.getPixelColor(Vector2i(1, 1));
    Color yellow = source.getPixelColor(Vector2i(0, 1));
    EXPECT_TRUE(r == Color(1.0, 0.0, 0.0));
    EXPECT_TRUE(g == Color(0.0, 1.0, 0.0));
    EXPECT_TRUE(b == Color(0.0, 0.0, 1.0));
    EXPECT_TRUE(yellow == Color(1.0, 1.0, 0.0));
}

TEST(Image, get_set_PixelColor_with_Interpolation)
{
    Image source;
    Image destination;

    int w = 3;
    int h = 3;
    const unsigned char v = 255;
    ByteArray sourceData(std::string(w * h * 3 * 2, v));

    //create a 3x3 white image
    source.setData(w, h, iifRgbUint16, sourceData.constData());
    //source.save(getCurrentFolderPath() + "/white4x4.png", Image::wfPng);

    // modify tge image so we have black, red, green and blue in each
    // corner and a bit of interpolation in between

    source.setPixelColor(Math::Vector2i(0, 0), Color(0.0, 0.0, 0.0));
    source.setPixelColor(Math::Vector2i(1, 0), Color(127 / 255.0, 0.0, 0.0));
    source.setPixelColor(Math::Vector2i(2, 0), Color(1.0, 0.0, 0.0));

    source.setPixelColor(Math::Vector2i(0, 1), Color(0.0, 0.0, 127 / 255.0));
    source.setPixelColor(Math::Vector2i(1, 1), Color(1.0, 1.0, 1.0));
    source.setPixelColor(Math::Vector2i(2, 1), Color(0.5, 0.5, 0.0));

    source.setPixelColor(Math::Vector2i(0, 2), Color(0.0, 0.0, 1.0));
    source.setPixelColor(Math::Vector2i(1, 2), Color(0.0, 127 / 255.0, 127 / 255.0));
    source.setPixelColor(Math::Vector2i(2, 2), Color(0.0, 1.0, 0.0));
    source.saveAs(getCurrentFolderPath() + "/coloredCorners2_16.png", Image::wfPng);

    //get the pixel value with interpolated colors
    Color center = source.getPixelColor(Vector2(1.5, 1.5));
    Color bl = source.getPixelColor(Vector2(1.25, 1.25));
    Color br = source.getPixelColor(Vector2(1.75, 1.25));
    Color tr = source.getPixelColor(Vector2(1.75, 1.75));
    Color tl = source.getPixelColor(Vector2(1.25, 1.75));

    Color t = source.getPixelColor(Vector2(1.5, 2.0));
    Color b = source.getPixelColor(Vector2(1.5, 1.0));
    Color l = source.getPixelColor(Vector2(1.0, 1.5));
    Color r = source.getPixelColor(Vector2(2.0, 1.5));

    // test center
    EXPECT_TRUE(center == Color(1.0, 1.0, 1.0));

    // test 4 positions around center
    EXPECT_TRUE(bl.getRgbUint8() == ColorRgbUint8(167, 143, 167));
    EXPECT_TRUE(br.getRgbUint8() == ColorRgbUint8(207, 167, 143));
    EXPECT_TRUE(tr.getRgbUint8() == ColorRgbUint8(167, 207, 167));
    EXPECT_TRUE(tl.getRgbUint8() == ColorRgbUint8(143, 167, 207));

    // test position directly on pixel edges
    EXPECT_TRUE(t.getRgbUint8() == ColorRgbUint8(127, 191, 191));
    EXPECT_TRUE(b.getRgbUint8() == ColorRgbUint8(191, 127, 127));
    EXPECT_TRUE(l.getRgbUint8() == ColorRgbUint8(127, 127, 191));
    EXPECT_TRUE(r.getRgbUint8() == ColorRgbUint8(191, 191, 127));
}

TEST(Image, heightmap_to_raw)
{
    Image heightMap;
    ByteArray ba;
    const int w = 512;
    const int h = 5;
    const int numBytes = 2;
    const int numChannels = 3; 
    ba.resize(w * h * numChannels * numBytes);

    heightMap.setData(w, h, iifRgbInt16, ba.constData());

    for(int j = 0; j < h; ++j)
        for (int i = 0; i < w; ++i)
        {
            Color c;
            c.setRedInt16((int16_t)(-255 + i));
            c.setGreenInt16((int16_t)(-255 + i));
            c.setBlueInt16((int16_t)(-255 + i));
            const Vector2i pixel(i, j);
            heightMap.setPixelColor(pixel, c);
        }

    heightMap.saveAs(getCurrentFolderPath() + "/heightMapTest.raw", Image::wfRaw);

    // reload image and check data...
    heightMap.load();

    for (int j = 0; j < h; ++j)
        for (int i = 0; i < w; ++i)
        {
            const Vector2i pixel(i, j);
            Color c = heightMap.getPixelColor(pixel);
            
            EXPECT_EQ(c.getRedInt16(), -255 + i);
            EXPECT_EQ(c.getGreenInt16(), -255 + i);
            EXPECT_EQ(c.getBlueInt16(), -255 + i);
        }
}

TEST(Image, heightmap_in_float16)
{
    using namespace half_float;

    ByteArray ba;
    ImageInternalFormat iif = iifRF16;
    const int w = 512, h = 512;
    const int numChannels = getNumberOfChannels(iif);
    const int numBytesPerchannel = getBytesPerChannel(iif);

    ba.resize(w*h*numChannels*numBytesPerchannel);
    Image im;
    im.setData(w, h, iif, ba.constData());

    // store a nice radial gradient...
    for (int y = 0; y < h; ++y)
        for (int x = 0; x < w; ++x)
        {
            const double nx = (x - (w/2.0)) / (w/2.0);
            const double ny = (y - (h / 2.0)) / (h / 2.0);

            if ((nx*nx + ny*ny) <= 1.0)
            {
                Color c;
                c.setRed( 22.0 - 22.0*Vector2(nx,ny).norm() );
                im.setPixelColor(Vector2i(x, y), c);
            }
            
        }

    bool saved = im.saveAs(getAssetsPath() + "/RF16_radial_gradient_from_0_to_22.raw", Image::wfRaw);
    EXPECT_TRUE(saved);
}

TEST(Image, RF16_images)
{
    using namespace half_float;

    ByteArray ba;
    ImageInternalFormat iif = iifRF16;
    const int w = 4, h = 4;
    const int numChannels = getNumberOfChannels(iif);
    const int numBytesPerchannel = getBytesPerChannel(iif);

    ba.resize(w*h*numChannels*numBytesPerchannel);
    Image im;
    im.setData(w, h, iif, ba.constData());

    // store a float16 value..
    const half v = (half)129.47f;
    for (int y = 0; y < h; ++y)
        for (int x = 0; x < w; ++x)
        {
            Color c;
            c.setRed(v);
            im.setPixelColor(Vector2i(x, y), c);
        }

    bool saved = im.saveAs(getAssetsPath() + "/RF16.raw", Image::wfRaw);
    EXPECT_TRUE(saved);

    // reload the image and ensure we can read the value
    im.load();
    for (int y = 0; y < h; ++y)
        for (int x = 0; x < w; ++x)
        {
            Color c = im.getPixelColor(Vector2i(x, y));
            EXPECT_TRUE(c.getRedF16() == v);
        }
}

TEST(Image, RgbaF16_images)
{
    using namespace half_float;
    using namespace half_float::literal;

    ByteArray ba;
    ImageInternalFormat iif = iifRgbaF16;
    const int w = 4, h = 4;
    const int numChannels = getNumberOfChannels(iif);
    const int numBytesPerchannel = getBytesPerChannel(iif);

    ba.resize(w*h*numChannels*numBytesPerchannel);
    Image im;
    im.setData(w, h, iif, ba.constData());

    // store a float16 value
    const half r = -std::numeric_limits<half>::max();
    const half g = -std::numeric_limits<half>::max() + 1.0_h;
    const half b = std::numeric_limits<half>::max() - 1.0_h;
    const half a = std::numeric_limits<half>::max();
    for (int y = 0; y < h; ++y)
        for (int x = 0; x < w; ++x)
        {
            Color c(r, g, b, a);
            im.setPixelColor(Vector2i(x, y), c);
        }

    bool saved = im.saveAs(getAssetsPath() + "/RgbaF16.raw", Image::wfRaw);
    EXPECT_TRUE(saved);

    // reload the image and ensure we can read the value
    im.load();
    for (int y = 0; y < h; ++y)
        for (int x = 0; x < w; ++x)
        {
            Color c = im.getPixelColor(Vector2i(x, y));
            EXPECT_TRUE(c.getRedF16() == r);
            EXPECT_TRUE(c.getGreenF16() == g);
            EXPECT_TRUE(c.getBlueF16() == b);
            EXPECT_TRUE(c.getAlphaF16() == a);
        }
}

TEST(Image, RF32_images)
{
    ByteArray ba;
    ImageInternalFormat iif = iifRF32;
    const int w = 4, h = 4;
    const int numChannels = getNumberOfChannels(iif);
    const int numBytesPerchannel = getBytesPerChannel(iif);

    ba.resize(w*h*numChannels*numBytesPerchannel);
    Image im;
    im.setData(w, h, iif, ba.constData());

    // store a float32 value..
    const float v = 129.47f;
    for (int y = 0; y < h; ++y)
        for (int x = 0; x < w; ++x)
        {
            Color c;
            c.setRed(v);
            im.setPixelColor(Vector2i(x, y), c);
        }
    
    bool saved = im.saveAs( getAssetsPath() + "/RF32.raw", Image::wfRaw );
    EXPECT_TRUE(saved);

    // reload the image and ensure we can read the value
    im.load();
    for (int y = 0; y < h; ++y)
        for (int x = 0; x < w; ++x)
        {
            Color c = im.getPixelColor(Vector2i(x,y));
            EXPECT_EQ(c.getRedF32(), v);
        }
}

TEST(Image, RgbaF32_images)
{
    ByteArray ba;
    ImageInternalFormat iif = iifRgbaF32;
    const int w = 4, h = 4;
    const int numChannels = getNumberOfChannels(iif);
    const int numBytesPerchannel = getBytesPerChannel(iif);

    ba.resize(w*h*numChannels*numBytesPerchannel);
    Image im;
    im.setData(w, h, iif, ba.constData());

    // store a float32 value..
    const float r = -127.65f;
    const float g = -4.3f;
    const float b = 16.8975f;
    const float a = 16899.00f;
    for (int y = 0; y < h; ++y)
        for (int x = 0; x < w; ++x)
        {
            Color c(r, g, b, a);
            im.setPixelColor(Vector2i(x, y), c);
        }

    bool saved = im.saveAs(getAssetsPath() + "/RgbaF32.raw", Image::wfRaw);
    EXPECT_TRUE(saved);

    // reload the image and ensure we can read the value
    im.load();
    for (int y = 0; y < h; ++y)
        for (int x = 0; x < w; ++x)
        {
            Color c = im.getPixelColor(Vector2i(x, y));
            EXPECT_EQ(c.getRedF32(), r);
            EXPECT_EQ(c.getGreenF32(), g);
            EXPECT_EQ(c.getBlueF32(), b);
            EXPECT_EQ(c.getAlphaF32(), a);
        }
}