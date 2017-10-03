
#include <cassert>
#include "CommonMath.h"
#include "FileInfo.h"
#include "HgtImage.h"
#include <regex>
#include "StreamUtility.h"

using namespace Realisim;
    using namespace Core;
using namespace std;

namespace
{
    // SRTM1
    const int kSrtm1NumberOfSamplePerLines = 3601;
    const int kSrtm1NumberOfLines = 3601;
    const double kSrtm1ResolutionPerSampleInDegree = degreesMinutesSecondsToDegrees(0, 0, 1);
    const double kSrtm1CoverageInLatLon = kSrtm1ResolutionPerSampleInDegree * kSrtm1NumberOfLines;
    const int kSrtm1FileSize = kSrtm1NumberOfSamplePerLines * kSrtm1NumberOfLines * 2;

    // SRTM3
    const int kSrtm3NumberOfSamplePerLines = 1201;
    const int kSrtm3NumberOfLines = 1201;    
    const double kSrtm3ResolutionPerSampleInDegree = degreesMinutesSecondsToDegrees(0, 0, 3);
    const double kSrtm3CoverageInLatLon = kSrtm3ResolutionPerSampleInDegree * kSrtm3NumberOfLines;
    const int kSrtm3FileSize = kSrtm3NumberOfSamplePerLines * kSrtm3NumberOfLines * 2;
}

//------------------------------------------------------------------------------
HgtImage::HgtImage() :
mFilenamePath(),
mImageData(),
mType(tUndefined),
mIsValid(false),
mBytesPerChannel(0),
mWidthInSample(0),
mHeightInSample(0),
mNumberOfChannels(0),
mResolutionPerSampleInDegrees(0.0),
mGeoTiffLowerLeftCorner(),
mGeoTiffUpperRightCorner()
{}

//------------------------------------------------------------------------------
HgtImage::~HgtImage()
{
    clear();
}

//------------------------------------------------------------------------------
void HgtImage::clear()
{
    mImageData = ByteArray();
    
    mIsValid = false;
    mType = tUndefined;
    mBytesPerChannel = 0;
    mWidthInSample = 0;
    mHeightInSample = 0;
    mNumberOfChannels = 0;
    mResolutionPerSampleInDegrees = 0.0;

    // geo referenced data
    mGeoTiffLowerLeftCorner = Math::Vector2();
    mGeoTiffUpperRightCorner = Math::Vector2();
}

//----------------------------------------------------------------------------
// flips the image 180 deg around the x axis
//
void HgtImage::flipVertical()
{
    const uint64_t lineSize = getWidthInSample() * getNumberOfChannels() * getBytesPerChannel();
    uint8_t *pTemp = new uint8_t[lineSize];

    uint8_t *pTop = nullptr, *pBottom = nullptr;
    uint8_t *data = (uint8_t*)mImageData.data();

    const int h = getHeightInSample();
    const int h2 = h >> 1;
    for (int y = 0; y < h2; ++y)
    {
        pTop = data + y * lineSize;
        pBottom = data + (h - 1 - y) * lineSize;

        memcpy(pTemp, pBottom, lineSize);
        memcpy(pBottom, pTop, lineSize);
        memcpy(pTop, pTemp, lineSize);
    }

    delete pTemp;
}

//------------------------------------------------------------------------------
int HgtImage::getBytesPerChannel() const
{ return mBytesPerChannel; }

//------------------------------------------------------------------------------
const std::string& HgtImage::getFilenamePath() const
{ return mFilenamePath; }

//------------------------------------------------------------------------------
ByteArray HgtImage::getImageData() const
{ return mImageData; }

//------------------------------------------------------------------------------
ImageInternalFormat HgtImage::getInternalFormat() const
{
    return iifRInt16;
}

//------------------------------------------------------------------------------
// Since hgt format has no header and embeds the lower left corner in the file
// name. this is a convenient function to retrieve the geodetic coordinate.
//
Math::Vector2 HgtImage::getLowerLeftCornerFromFileName(const std::string& iFilename, bool *oSuccess)
{
    assert(oSuccess);
    //GeodeticCoordinate r(0.0, 0.0, 0.0, Datum::WGS84);
    Math::Vector2 r;
    *oSuccess = false;
    
    enum direction {dNorth = 0 , dSouth, dEast, dWest};
    // 4 groups
    //
    regex matchRe("([NS]{1})([0-9]+)([WE]{1})([0-9]+)");
    smatch m;
    std::regex_search(iFilename, m, matchRe);
    if (m.size() == 5)
    {
        const direction ns = m[1].str() == "N" ? dNorth : dSouth;
        const direction ew = m[3].str() == "E" ? dEast : dWest;
        int lat = stoi(m[2].str()) * (ns == dNorth ? 1 : -1);
        int lon = stoi(m[4].str()) * (ew == dEast ? 1 : -1);

        //r.setLatitude(lat);
        //r.setLongitude(lon);
        r.set(lon, lat);
        *oSuccess = true;
    }    
    return r;
}

//------------------------------------------------------------------------------
// returns the lower left corner (in long/lat) of the image
// If the image has no geo reference data associated, it will return a
// (0, 0, 0) geodetic coordinate
//
Math::Vector2 HgtImage::getGeoTiffLowerLeftCorner() const
{
    return mGeoTiffLowerLeftCorner;
}

//------------------------------------------------------------------------------
// returns the upper right corner (in long/lat) of the image
// If the image has no geo reference data associated, it will return a
// (0, 0, 0) geodetic coordinate
//
Math::Vector2 HgtImage::getGeoTiffUpperRightCorner() const
{
    return mGeoTiffUpperRightCorner;
}

//------------------------------------------------------------------------------
int HgtImage::getHeightInSample() const
{
    return mHeightInSample;
}

//------------------------------------------------------------------------------
int HgtImage::getNumberOfChannels() const
{ return mNumberOfChannels; }

//------------------------------------------------------------------------------
double HgtImage::getResolutionPerSampleInDegrees() const
{
    return mResolutionPerSampleInDegrees;
}

//------------------------------------------------------------------------------
HgtImage::type HgtImage::getType() const
{
    return mType;
}

//------------------------------------------------------------------------------
Math::Vector2i HgtImage::getSizeInPixel() const
{
    return Math::Vector2i(mWidthInSample, mHeightInSample);
}

//------------------------------------------------------------------------------
uint64_t HgtImage::getSizeInBytes() const
{
    return (uint64_t)mBytesPerChannel * 
        (uint64_t)mWidthInSample * 
        (uint64_t)mHeightInSample * 
        (uint64_t)mNumberOfChannels;
}

//------------------------------------------------------------------------------
int HgtImage::getWidthInSample() const
{ return mWidthInSample; }

//------------------------------------------------------------------------------
bool HgtImage::hasImageData() const
{ return !mImageData.isEmpty(); }

//------------------------------------------------------------------------------
bool HgtImage::isValid() const
{ return mIsValid; }

//------------------------------------------------------------------------------
void HgtImage::load()
{    
    loadHeader();
    if (isValid())
    {
        mImageData = readFromFile(getFilenamePath());

        // byte order in HGT is BIG ENDIAN, so we will untangle all that to be
        // in the right order here.

        // To go from Big endian to little, if necessary, we will swap bytes
        // of every short.
        //
        if (StreamUtility::getLocalMachineByteOrder() != StreamUtility::eBigEndian)
        {
            uint64_t numberOfSamples = (uint64_t)getWidthInSample() * (uint64_t)getHeightInSample();
            char *pCursor = (char*)mImageData.data();
            char *pCursorPlusOne = nullptr;
            for (size_t i = 0; i < numberOfSamples; ++i)
            {
                pCursorPlusOne = pCursor + 1;

                // swap the byte
                *pCursor ^= *pCursorPlusOne;
                *pCursorPlusOne ^= *pCursor;
                *pCursor ^= *pCursorPlusOne;

                // move up one short.
                pCursor++;
                pCursor++;
            }
        }

        flipVertical();

        mIsValid = !mImageData.isEmpty();
    }

    if(!isValid())
    {
        clear();
    }
}

//------------------------------------------------------------------------------
void HgtImage::loadHeader()
{
    mIsValid = false;
    
    mBytesPerChannel = 2;

    // from the file size we will determine if it is a srtm1 or srtm3, 
    // from the file name we will find srtm_ramp2.world.86400x43200 provided with the 
    // blue marble kit.
    // else we dont know what it is, unless it was set as a custom using 
    FileInfo fi(getFilenamePath());
    if (fi.getSize() == kSrtm1FileSize)
    { mType = tSrtm1; }

    if (fi.getSize() == kSrtm3FileSize)
    { mType = tSrtm3; }

    if (getFilenamePath().find("srtm_ramp2.world.86400x43200") != string::npos)
    { mType = tBlueMarbleSrtmRamp2; }

    switch (mType)
    {
    case tSrtm1:
        mGeoTiffLowerLeftCorner = getLowerLeftCornerFromFileName(getFilenamePath(), &mIsValid);

        mWidthInSample = kSrtm1NumberOfSamplePerLines;
        mHeightInSample = kSrtm1NumberOfLines;
        mResolutionPerSampleInDegrees = kSrtm1ResolutionPerSampleInDegree;

        mGeoTiffUpperRightCorner.setX(mGeoTiffLowerLeftCorner.x() + kSrtm1CoverageInLatLon);
        mGeoTiffUpperRightCorner.setY(mGeoTiffLowerLeftCorner.y() + kSrtm1CoverageInLatLon);
        //mGeoTiffUpperRightCorner.setLongitude(mGeoTiffLowerLeftCorner.getLongitude() + kSrtm1CoverageInLatLon);
        //mGeoTiffUpperRightCorner.setLatitude(mGeoTiffLowerLeftCorner.getLatitude() + kSrtm1CoverageInLatLon);

        break;
    case tSrtm3:
        mGeoTiffLowerLeftCorner = getLowerLeftCornerFromFileName(getFilenamePath(), &mIsValid);

        mWidthInSample = kSrtm3NumberOfSamplePerLines;
        mHeightInSample = kSrtm3NumberOfLines;
        mResolutionPerSampleInDegrees = kSrtm3ResolutionPerSampleInDegree;

        mGeoTiffUpperRightCorner.setX(mGeoTiffLowerLeftCorner.x() + kSrtm3CoverageInLatLon);
        mGeoTiffUpperRightCorner.setY(mGeoTiffLowerLeftCorner.y() + kSrtm3CoverageInLatLon);
//        mGeoTiffUpperRightCorner.setLongitude(mGeoTiffLowerLeftCorner.getLongitude() + kSrtm3CoverageInLatLon);
//        mGeoTiffUpperRightCorner.setLatitude(mGeoTiffLowerLeftCorner.getLatitude() + kSrtm3CoverageInLatLon);
        break;
    case tBlueMarbleSrtmRamp2:
        mWidthInSample = 86400;
        mHeightInSample = 43200;
        mResolutionPerSampleInDegrees = 1 / 240.0;
        //mGeoTiffLowerLeftCorner = GeodeticCoordinate(-90.0, -180.0, 0.0, Datum::WGS84);
        
        mGeoTiffLowerLeftCorner.set(-180.0, -90.0);
        mGeoTiffUpperRightCorner.setX(mGeoTiffLowerLeftCorner.x() + (mWidthInSample * mResolutionPerSampleInDegrees));
        mGeoTiffUpperRightCorner.setY(mGeoTiffLowerLeftCorner.y() + (mHeightInSample * mResolutionPerSampleInDegrees));
        //mGeoTiffUpperRightCorner.setLongitude(mGeoTiffLowerLeftCorner.getLongitude() + (mWidthInSample * mResolutionPerSampleInDegrees));
        //mGeoTiffUpperRightCorner.setLatitude(mGeoTiffLowerLeftCorner.getLatitude() + (mHeightInSample * mResolutionPerSampleInDegrees));
        mIsValid = true;
    break;
    case tCustom: 
        mIsValid = true;
        // custom type header is completely handle in setCustomType.
        break;
    default: assert(0); break;
    }

    mNumberOfChannels = 1;

    if (!isValid()) { clear(); }
}

//------------------------------------------------------------------------------
void HgtImage::setFilenamePath(const std::string& iV)
{ 
    clear();
    mFilenamePath = iV;
}

//------------------------------------------------------------------------------
// This method is used to fully define the format of the image to be read.
// Ex:
//      HgtImage im;
//      im.setFilenamePath("./topography/srtm_ramp2.world.86400x43200.bin");
//      im.setCustomType(86400, 43200, 1 / 240.0, GeodeticCoordinate(-90.0, -180.0, 0.0, Datum::WGS84));
//      im.load();
//
void HgtImage::setCustomType(int iNumberOfSamplesPerLine, int iNumberOfLines, double iResolutionPerSampleInDegree,
                             Math::Vector2 iLowerLeftCorner)
{
    mType = tCustom;
    mWidthInSample = iNumberOfSamplesPerLine;
    mHeightInSample = iNumberOfLines;
    mResolutionPerSampleInDegrees = iResolutionPerSampleInDegree;
    mGeoTiffLowerLeftCorner = iLowerLeftCorner;
    mGeoTiffUpperRightCorner.setX(mGeoTiffLowerLeftCorner.x() + (mWidthInSample * mResolutionPerSampleInDegrees));
    mGeoTiffUpperRightCorner.setY(mGeoTiffLowerLeftCorner.y() + (mHeightInSample * mResolutionPerSampleInDegrees));
}
