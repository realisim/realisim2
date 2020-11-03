#include "FileInfo.h"
#include "gtest/gtest.h"
#include "ImageSupport/HgtImage.h"
#include "Math/IsEqual.h"
#include "Math/Vector.h"
#include "Path.h"

using namespace Realisim;
using namespace Core;
using namespace Math;

namespace
{

    std::string getAssetsPath()
    {
        // return path to executable
        FileInfo fi(Path::getApplicationFilePath());
        return fi.getCanonicalPath() + "/../CoreAssets";
    }

}

TEST(HgtImage, testHgtImageTypeSrtm3)
{
    HgtImage im;
    im.setFilenamePath(getAssetsPath() + "/N45W072.hgt");
    im.loadHeader();
    EXPECT_TRUE(im.isValid());

    EXPECT_EQ(im.getBytesPerChannel(), 2);
    EXPECT_STREQ(im.getFilenamePath().c_str(), (getAssetsPath() + "/N45W072.hgt").c_str());
    EXPECT_EQ(im.getGeoTiffLowerLeftCorner().y(), 45);
    EXPECT_EQ(im.getGeoTiffLowerLeftCorner().x(), -72);

    EXPECT_EQ(im.getGeoTiffUpperRightCorner().y(), 45 + (1201 * im.getResolutionPerSampleInDegrees()) );
    EXPECT_EQ(im.getGeoTiffUpperRightCorner().x(), -72 + (1201 * im.getResolutionPerSampleInDegrees()));

    EXPECT_EQ(im.getHeightInPixel(), 1201);
    EXPECT_EQ(im.getNumberOfChannels(), 1);
    EXPECT_EQ(im.getResolutionPerSampleInDegrees(), 3/3600.0);
    EXPECT_EQ(im.getType(), HgtImage::tSrtm3);
    EXPECT_EQ(im.getWidthInPixel(), 1201);
    
    im.load();
    EXPECT_TRUE(im.isValid());
    EXPECT_EQ(im.getSizeInBytes(), 1201*1201*2);
}

TEST(HgtImage, testHgtImageCustomType)
{
    const int kNumSamplePerLines = 8;
    const int kNumLines = 1;

    HgtImage im;
    im.setFilenamePath(getAssetsPath() + "/customSrtm_8_samples.bin");
    im.setCustomType(kNumSamplePerLines, kNumLines, 1/(double)kNumSamplePerLines, Vector2(0.0, 0.0));
    im.loadHeader();
    EXPECT_TRUE(im.isValid());

    EXPECT_EQ(im.getBytesPerChannel(), 2);

    EXPECT_EQ(im.getGeoTiffLowerLeftCorner().x(), 0);
    EXPECT_EQ(im.getGeoTiffLowerLeftCorner().y(), 0);

    EXPECT_EQ(im.getGeoTiffUpperRightCorner().x(), 0 + (kNumSamplePerLines * im.getResolutionPerSampleInDegrees()));
    EXPECT_EQ(im.getGeoTiffUpperRightCorner().y(), 0 + (kNumLines * im.getResolutionPerSampleInDegrees()));

    EXPECT_EQ(im.getHeightInPixel(), kNumLines);
    EXPECT_EQ(im.getNumberOfChannels(), 1);
    EXPECT_EQ(im.getResolutionPerSampleInDegrees(), 1 / (double)kNumSamplePerLines);
    EXPECT_EQ(im.getType(), HgtImage::tCustom);
    EXPECT_EQ(im.getWidthInPixel(), kNumSamplePerLines);

    im.load();
    EXPECT_TRUE(im.isValid());
    EXPECT_EQ(im.getSizeInBytes(), kNumSamplePerLines * kNumLines * 2);

    //poke around and check some values...

    // Bytes in the sample file are has follow (it was edited using HxD)
    // The layout of the HGT file is big endian...
    // Remember that under Windows the memory layout is little endian
    // 
    // Anyway, the content of the file is has follow and is BIG ENDIAN
    // 00 01 | 00 02 | 00 03 | 00 04 | 01 00 | 10 00 | FF FF | F0 00
    //
    // which should result in the following memory layout in little endian.
    // 01 00 | 02 00 | 03 00 | 04 00 | 00 01 | 00 10 | FF FF | 00 F0

    const short *pShort = (const short*)im.getImageData().constData();
    short values[8];
    short compareTo[8] = { 1, 2, 3, 4, 256, 4096, -1, -4096};

    for (int i = 0; i < 8; ++i)
    {
        values[i] = *pShort;
        ++pShort;

        EXPECT_EQ(values[i], compareTo[i]);
    }
}

// This test is disabled, it is there only to test the loading of
// a 7GB srtm image.
//
TEST(DISABLED_HgtImage, loadsrtm_ramp2_world)
{
    HgtImage im;
    im.setFilenamePath("F:/work/Project/Commando/NASA - blue marble/topography/srtm_ramp2.world.86400x43200.bin");
    im.setCustomType(86400, 43200, 1 / 240.0, Vector2(-180.0, -90.0));
    im.loadHeader();
    EXPECT_TRUE(im.isValid());
    
    im.load();
    EXPECT_TRUE(im.isValid());
}