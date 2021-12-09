
#include <algorithm>
#include <cassert>
#include "FileInfo.h"
#include "Half/half.hpp"
#include "Image.h"
#include "ImageSupport/ImageBufferHelpers.h"
#include "Math/Conversion.h"
#include "Math/Interpolation.h"

using namespace Realisim;
    using namespace Core;
    using namespace Math;
using namespace half_float;
using namespace std;

//-----------------------------------------------------------------------------
Image::Image() :
    mFilenamePath(),
    mImageData(),
    mSizeInPixel(),
    mSizeInBytes(0),
    mInternalFormat(iifUndefined),
    mIsValid(false)
{
}

//-----------------------------------------------------------------------------
Image::Image(const std::string& iFilenamePath) :
    mFilenamePath(iFilenamePath),
    mImageData(),
    mSizeInPixel(),
    mSizeInBytes(0),
    mInternalFormat(iifUndefined),
    mIsValid(false)
{
}

//-----------------------------------------------------------------------------
Image::~Image()
{
    clear();
}

//-----------------------------------------------------------------------------
// This method will apply the configured wrap type to iPixel and returns the
// wrapped value into iPx and iPy.
//
// The method also returns a boolean to indicate success/failure of the wrapping.
// For example with wrap mode wtClampToBorder, the wrapping will fail if iPixel is
// outside the width/height of the image. Since the returned values are uint64_t, it
// is not possible to return negative values, so the boolean is there to indicate
// the failure.
//
bool Image::applyWrapType(int iX, int iY, uint64_t *iPx, uint64_t *iPy) const
{
    bool r = true;
    // py and px are uint64_t to enable the byteOffset to be truly a uint64 
    // thus enabling larger than 4Gb images..
    //

    int64_t x = 0, y = 0;
    const int w = getWidth();
    const int h = getHeight();

    switch (getWrapType())
    {
    case wtClampToBorder:
        x = iX;
        y = iY;

        r = x >= 0 && x < w && y >= 0 && y < h;
        break;
    case wtClampToEdge:
        x = indexToClampedToEdgeIndex(iX, w);
        y = indexToClampedToEdgeIndex(iY, h);
        break;
    case wtRepeat:
        x = indexToPeriodicIndex(iX, w);
        y = indexToPeriodicIndex(iY, h);
        break;
    default: assert(0); break;
    }

    *iPx = x;
    *iPy = y;

    return r;
}

//-----------------------------------------------------------------------------
void Image::clear()
{
    // mFilenamePath is explicit left alone when clear is called.
    //
    // clear is called to release internal buffer and associated metadata. The
    // reseaon why we leave mFilenamePath and mFormat is to be able to do the
    // following.:
    //
    //  Image im.
    //  im.set("somePath", fPng);
    //  im.load(); // load call a clear but needs the path and the format...
    //

    //mFilenamePath = string();
    mImageData.clear();
    mSizeInPixel.set(0, 0);
    mSizeInBytes = 0;
    mInternalFormat = iifUndefined;
    mIsValid = false;
}

//----------------------------------------------------------------------------
// Flips vertically (180 deg around x axis) the content of the ByteArray iBa.
// Iba must have uncompressed data.
//
void Image::flipVertical()
{
    flipVertical(mImageData);
}

//----------------------------------------------------------------------------
// see Image::flipVertical()
//
void Image::flipVertical(ByteArray &iBa)
{
	//early out.
	if (iBa.isEmpty()) return;

    const int lineSize = getWidth() * getNumberOfChannels() * getBytesPerChannel();
    uint8_t *pTemp = new uint8_t[lineSize];

    uint8_t *pTop = nullptr, *pBottom = nullptr;
    uint8_t *data = (uint8_t*)iBa.data();

    const int h = getHeight();
    const int h2 = h >> 1;
    for (int y = 0; y < h2 ; ++y)
    {
        pTop = data + y * lineSize;
        pBottom = data + (h - 1 - y) * lineSize;

        memcpy(pTemp, pBottom, lineSize);
        memcpy(pBottom, pTop, lineSize);
        memcpy(pTop, pTemp, lineSize);
    }

    delete pTemp;
}

//----------------------------------------------------------------------------
int Image::getBytesPerChannel() const
{
    return Core::getBytesPerChannel(getInternalFormat());
}

//----------------------------------------------------------------------------
std::string Image::getExtensionFromFormat(Format iFormat)
{
    std::string r = "unknown";
    switch (iFormat)
    {
    case Image::fUnsupported:
        break;
    case Image::fRaw: r = "raw"; break;
    case Image::fRgb: r = "rgb"; break;
//    case Image::fDds: r = "dds"; break;
    case Image::fPng: r = "png"; break;
//    case Image::fTiff: r = "tif"; break;
    case Image::fHgt: r = "hgt"; break;
    case Image::fTga: r = "tga"; break;
    default: assert(0);  break;
    }
    return r;
}

//----------------------------------------------------------------------------
std::string Image::getFilenamePath() const
{
    return mFilenamePath;
}

//----------------------------------------------------------------------------
Image::Format Image::getFormatFromExtension(const std::string& iExtension)
{
    Image::Format r = fUnsupported;

    if (_stricmp(iExtension.c_str(), "raw") == 0)
    { r = fRaw; }
    else if (_stricmp(iExtension.c_str(), "rgb") == 0)
    { r = fRgb; }
    else if (_stricmp(iExtension.c_str(), "rgba") == 0)
    { r = fRgb; }
    /*else if (_stricmp(iExtension.c_str(), "dds") == 0)
    { r = fDds; }*/
    else if (_stricmp(iExtension.c_str(), "png") == 0)
    { r = fPng; }
    /*else if (_stricmp(iExtension.c_str(), "tif") == 0 ||
        _stricmp(iExtension.c_str(), "tiff") == 0)
    { r = fTiff; }*/
    else if (_stricmp(iExtension.c_str(), "hgt") == 0)
    { r = fHgt; }
    else if (_stricmp(iExtension.c_str(), "tga") == 0)
    { r = fTga; }
    else { r = fUnsupported; }

    return r;
}

//----------------------------------------------------------------------------
Image::Format Image::getFormatFromFilename(const std::string& iFilename)
{
    FileInfo fi;
    fi.setCached(false);
    fi.setFile(iFilename);

    return Image::getFormatFromExtension(fi.getSuffix());
}

//----------------------------------------------------------------------------
int Image::getHeight() const
{
    return mSizeInPixel.y();
}

//----------------------------------------------------------------------------
ImageInternalFormat Image::getInternalFormat() const
{
    return mInternalFormat;
}

//----------------------------------------------------------------------------
ByteArray Image::getImageData() const
{
    return mImageData;
}

//----------------------------------------------------------------------------
int Image::getNumberOfChannels() const
{
    return Core::getNumberOfChannels(getInternalFormat());
}

//----------------------------------------------------------------------------
// Returns the Color at pixel iPixel.
// If pixel is out of bound, we clamp to border...
// No interpolation is happening, so nearest interpolation
//
Color Image::getPixelColor(int iX, int iY) const
{
    Color r;

    if (isValid())
    {
        uint64_t px, py;
        if (applyWrapType(iX, iY, &px, &py))
        {
            const int w = getWidth();

            const uint64_t byteOffset = ((py * w) + px) * getBytesPerChannel() * getNumberOfChannels();
            const char *pPtr = mImageData.constData() + byteOffset;

            r = getColorFromImageBuffer(pPtr, getInternalFormat());
        }
    }
    return r;
}

//----------------------------------------------------------------------------
Color Image::getPixelColor(const Math::Vector2i& iPixel) const
{
    return getPixelColor(iPixel.x(), iPixel.y());
}

//----------------------------------------------------------------------------
// see getPixelColor(Math::Vector2i iPixel) const
// The difference is that it performs a bilinear interpolation
//
// see https://en.wikipedia.org/wiki/Bilinear_interpolation
//
Color Image::getPixelColor(double iX, double iY) const
{
    //pixels that will take part in the bilinear interpolation
    // 4 pixels ordered in the following manner
    //
    // 0: bottomleft
    // 1: bottomright
    // 2: topRight
    // 3: topLeft
    //
    //  [3]           [2]
    //  0,1           1,1
    //   *-------------*
    //   |             |
    //   |        *p   |
    //   |             |
    //   |             |
    //   *-------------*
    //  0,0           1,0
    //  [0]            [1]
    //
    // Point p, is the x,y position in bilieaner interpolation space, it alsp 
    // represent the weight of each sample.
    //

    vector<Vector2> pixels = { Vector2(0.0, 0.0),
        Vector2(1.0, 0.0),
        Vector2(1.0, 1.0),
        Vector2(0.0, 1.0) };
    vector<Color> pixelColors(4, Color());
    vector<Vector2i> samplePositions(4, Vector2i(0));

    //define in which quadrant (from he center of the pixel)
    //the current sample pixel is and make the 4 color samples
    // required for the bilinear interpolation
    //
    const Vector2 pixel(iX, iY);
    const Vector2i pixelCenterI((int)iX, (int)iY);
    const Vector2 pixelCenter(pixelCenterI.x() + 0.5, pixelCenterI.y() + 0.5);
    const Vector2 d = pixel - pixelCenter;

    //left side
    if (d.x() <= 0)
    {
        //lower left
        if (d.y() <= 0)
        {
            samplePositions[0] = pixelCenterI + Vector2i(-1, -1);
            samplePositions[1] = pixelCenterI + Vector2i(0, -1);
            samplePositions[2] = pixelCenterI + Vector2i(0, 0);
            samplePositions[3] = pixelCenterI + Vector2i(-1, 0);
        }
        //upper left
        else if (d.y() > 0)
        {
            samplePositions[0] = pixelCenterI + Vector2i(-1, 0);
            samplePositions[1] = pixelCenterI + Vector2i(0, 0);
            samplePositions[2] = pixelCenterI + Vector2i(0, 1);
            samplePositions[3] = pixelCenterI + Vector2i(-1, 1);
        }
    }
    //right side
    else
    {
        //lower rigth (includes y == 0)!!!
        if (d.y() <= 0)
        {
            samplePositions[0] = pixelCenterI + Vector2i(0, -1);
            samplePositions[1] = pixelCenterI + Vector2i(1, -1);
            samplePositions[2] = pixelCenterI + Vector2i(1, 0);
            samplePositions[3] = pixelCenterI + Vector2i(0, 0);
        }
        // upper right
        else if (d.y() > 0)
        {
            samplePositions[0] = pixelCenterI + Vector2i(0, 0);
            samplePositions[1] = pixelCenterI + Vector2i(1, 0);
            samplePositions[2] = pixelCenterI + Vector2i(1, 1);
            samplePositions[3] = pixelCenterI + Vector2i(0, 1);
        }
    }

    // fetch all color samples
    for (int i = 0; i < 4; i++)
    {
        pixelColors[i] = getPixelColor(samplePositions[i].x(), samplePositions[i].y());
    }

    // since we created a normalized rectangle of interpolation, with
    // samplePosition[0] always being the lower left corner,
    // we compute de interpoation factor by substracting the requested
    // pixel position (pixel) to the center (origin) of the lower left
    // sampling corner.
    //
    const Vector2 dxdy = pixel - ((toVector2(samplePositions[0])) + Vector2(0.5)); //point of interpolation in samples normalized space
    return biLerp(pixelColors[0], pixelColors[1], pixelColors[2], pixelColors[3], fabs(dxdy.x()), fabs(dxdy.y()));
}

//----------------------------------------------------------------------------
Color Image::getPixelColor(const Math::Vector2& iPixel) const
{
    return getPixelColor(iPixel.x(), iPixel.y());
}

//----------------------------------------------------------------------------
Color Image::getPixelColor(const Math::Vector2& iPixel, PixelInterpolation iPi) const
{
    Color r;
    switch (iPi)
    {
    case piNearest: r = getPixelColor(Vector2i((int)iPixel.x(), (int)iPixel.y())); break;
    case piLinear: r = getPixelColor(iPixel); break;
    default: break;
    }
    return r;
}

//----------------------------------------------------------------------------
uint64_t Image::getSizeInBytes() const
{
    return mSizeInBytes;
}

//----------------------------------------------------------------------------
const Math::Vector2i& Image::getSizeInPixels() const
{
    return mSizeInPixel;
}

//----------------------------------------------------------------------------
int Image::getWidth() const
{
    return mSizeInPixel.x();
}

//----------------------------------------------------------------------------
Image::WrapType Image::getWrapType() const
{
    return mWrapType;
}

//----------------------------------------------------------------------------
Image::Format Image::guessFormatFromFileName() const
{
    return Image::getFormatFromFilename(getFilenamePath());
}

//----------------------------------------------------------------------------
// Returns true if the image data is not empty. Usually, when the image is
// loaded via load(...) there should be image data.
//
// If the image is loaded via loadHeader(...) then there will be no image data
// available but all meta information (width, height, sizeInBytes etc...) will
// be available. Also method isValid() will report true, unless there was an
// error.
//
bool Image::hasImageData() const
{
    return !mImageData.isEmpty();
}

//----------------------------------------------------------------------------
bool Image::isValid() const
{
    return mIsValid;
}

//----------------------------------------------------------------------------
// This will load the image, see method load(Format). The format will
// be deduced from the filename.
//
bool Image::load()
{
    bool r = false;
    clear();
    if (!getFilenamePath().empty())
    {
        r = load(guessFormatFromFileName(), false);
    }
    return r;
}

//----------------------------------------------------------------------------
bool Image::load(Format iFormat, bool iHeaderOnly)
{
    bool r = false;
    
    clear();

    bool flipVertical = false;
    IImageReader *reader = nullptr;
    switch (iFormat)
    {
	case fUnsupported: assert(0); break;
    case fRaw: reader = new RawImage(); break;
    case fRgb: reader = new RgbImage(); break;
        //    case fDds: r = loadDdsImage(getFilenamePath(), false); break;
    case fPng: 
#ifdef REALISIM_THIRDPARTIES_LODEPNG
        reader = new PngImage();
        flipVertical = true;
#endif // REALISIM_THIRDPARTIES_LODEPNG
        break;
        //	case fTiff: r = loadTiffImage(getFilenamePath(), false); break;
    case fHgt: reader = new HgtImage(); break;
    case fTga: 
#ifdef REALISIM_THIRDPARTIES_TGA
        reader = new TgaImage(); break;
#endif //#ifdef REALISIM_THIRDPARTIES_TGA
    default: assert(0); break;
    }

    if (reader)
    {
        r = load(reader, iHeaderOnly, flipVertical);
    }
    else
    {
        printf("Image::load - Warning - Image %s is not supported...\n", getFilenamePath().c_str());
    }

    mIsValid = r;
    return r;
}

//----------------------------------------------------------------------------
bool Image::load(IImageReader *ipReader, bool iHeaderOnly, bool iFlipVertical)
{
    bool r = false;
    const string &iFilename = getFilenamePath();
    ipReader->setFilenamePath(iFilename);
    iHeaderOnly ? ipReader->loadHeader() : ipReader->load();

    if (ipReader->isValid())
    {
        mFilenamePath = iFilename;
        mImageData = ipReader->getImageData();
        mSizeInPixel = ipReader->getSizeInPixel();
        mInternalFormat = ipReader->getInternalFormat();
        mSizeInBytes = ipReader->getSizeInBytes();

        if (iFlipVertical)
            flipVertical();
        r = true;
    }
    return r;
}

//----------------------------------------------------------------------------
bool Image::loadHeader()
{ 
    bool r = false;
    clear();
    if(!getFilenamePath().empty())
    {
        r = load(guessFormatFromFileName(), true);
    }
    return r;
}

//----------------------------------------------------------------------------
// Saves the current image to a file of WritableFormat format.
// Returns true if successfull. otherwise returns false.
//
// The image file path will be changed
// to the one provided (iFilenamePath). see getFilenamePath()
//
bool Image::saveAs(const std::string& iFilenamePath, WritableFormat iF)
{
    bool r = false;
    mFilenamePath = iFilenamePath;

    switch (iF)
    {   
	case wfPng:  r = savePngImage(iFilenamePath); break;
    case wfRaw: r = saveRawImage(iFilenamePath); break;
//    case wfDds:
//        r = saveDdsImage(iFilenamePath, DdsImage::SaveOptions() ); //default options...
//        break;
    default: assert(0); break;
    }

    return r;
}

//----------------------------------------------------------------------------
// Will save the uncompressed image data (mImageData) to png format.
// The bit depth and number of channels of the original image will be preserved.
//
bool Image::savePngImage(const std::string& iFilenamePath)
{
	//increment refCount on imagedata. So if flip is necessary, we wont modify the original data.
	//
	ByteArray ba = getImageData();

    flipVertical(ba);

	return PngImage::save(iFilenamePath,
		getWidth(),
		getHeight(),
		getBytesPerChannel(),
		getNumberOfChannels(),
		ba);
}

//----------------------------------------------------------------------------
// Saves the raw data to file specified by iFilenamePath
//
bool Image::saveRawImage(const std::string& iFilenamePath)
{    
    return RawImage::save(iFilenamePath, 
        getWidth(), getHeight(), 
        getInternalFormat(),
        getImageData() );
}

//----------------------------------------------------------------------------
void Image::set(const std::string& iFilenamePath)
{
    clear();
    mFilenamePath = iFilenamePath;
}

//----------------------------------------------------------------------------
void Image::set(int iWidth, int iHeight, ImageInternalFormat iIf)
{
    clear();
    
    mSizeInPixel.set(iWidth, iHeight);
    mInternalFormat = iIf;
    mSizeInBytes = (uint64_t)getWidth() *
        (uint64_t)getHeight() *
        (uint64_t)getNumberOfChannels() *
        (uint64_t)getBytesPerChannel();
    mImageData.resize(mSizeInBytes);
    
    mIsValid = true;
}

//----------------------------------------------------------------------------
void Image::set(const Math::Vector2i& iSize, ImageInternalFormat iIf)
{ set(iSize.x(), iSize.y(), iIf); }

//----------------------------------------------------------------------------
// this function will allocate storage for the binary paylaod. It copies
// all data pointer by ipData.
// if ipData is null, the data will be initialized to black (0).
//
void Image::setData(int iWidth, int iHeight, 
    ImageInternalFormat iIf,
    const char* ipData)
{
    clear();

    mSizeInPixel.set(iWidth, iHeight);
    mInternalFormat = iIf;
    mSizeInBytes = (uint64_t)getWidth() * 
        (uint64_t)getHeight() * 
        (uint64_t)getNumberOfChannels() * 
        (uint64_t)getBytesPerChannel();
   
    if (ipData != nullptr)
    { mImageData.set(ipData, getSizeInBytes()); }
    else
    { mImageData.resize(mSizeInBytes); }

    mIsValid = true;
}

//----------------------------------------------------------------------------
void Image::setFilenamePath(const std::string& iFilenamePath)
{
    mFilenamePath = iFilenamePath;
}

//----------------------------------------------------------------------------
// set the pixel at index iPixel to the color iCol
// no interpolation is performed.
//
// If iPixel is out of bounds, nothing happens
//
void Image::setPixelColor(Math::Vector2i iPixel, const Color& iCol)
{
    if (iPixel >= Vector2i(0, 0) && iPixel < getSizeInPixels())
    {
        // py and px are uint64_t to enable the byteOffset on the following
        // line to be truly a uint64 thus enabling larger than 4Gb images..
        //
        const uint64_t px = iPixel.x();
        const uint64_t py = iPixel.y();

        const uint64_t byteOffset = ((py * getWidth()) + px) * getBytesPerChannel() * getNumberOfChannels();
        char *ptr = mImageData.data() + byteOffset;

        switch (getInternalFormat())
        {
        case iifUndefined: break;
        case iifRUint8: *((uint8_t*)ptr) = iCol.getRedUint8(); break;
        case iifRInt8: *ptr = iCol.getRedInt8(); break;
        case iifRUint16: *((uint16_t*)ptr) = iCol.getRedUint16(); break;
        case iifRInt16: *((int16_t*)ptr) = iCol.getRedInt16(); break;
        case iifRF16: *((half*)ptr) = iCol.getRedF16(); break;
        case iifRUint32: *((uint32_t*)ptr) = iCol.getRedUint32(); break;
        case iifRInt32: *((int32_t*)ptr) = iCol.getRedInt32(); break;
        case iifRF32: *((float*)ptr) = iCol.getRedF32(); break;
        case iifRgbUint8: 
        {
            uint8_t *typedPtr = (uint8_t*)ptr;
            typedPtr[0] = iCol.getRedUint8(); 
            typedPtr[1] = iCol.getGreenUint8();
            typedPtr[2] = iCol.getBlueUint8();
        }break;
        case iifRgbInt8:
        {
            int8_t *typedPtr = (int8_t*)ptr;
            typedPtr[0] = iCol.getRedInt8();
            typedPtr[1] = iCol.getGreenInt8();
            typedPtr[2] = iCol.getBlueInt8();
        }break;
        case iifRgbUint16:
        {
            uint16_t *typedPtr = (uint16_t*)ptr;
            typedPtr[0] = iCol.getRedUint16();
            typedPtr[1] = iCol.getGreenUint16();
            typedPtr[2] = iCol.getBlueUint16();
        }break;
        case iifRgbInt16:
        {
            int16_t *typedPtr = (int16_t*)ptr;
            typedPtr[0] = iCol.getRedInt16();
            typedPtr[1] = iCol.getGreenInt16();
            typedPtr[2] = iCol.getBlueInt16();
        }break;
        case iifRgbF16:
        {
            half *typedPtr = (half*)ptr;
            typedPtr[0] = iCol.getRedF16();
            typedPtr[1] = iCol.getGreenF16();
            typedPtr[2] = iCol.getBlueF16();
        }break;
        case iifRgbUint32:
        {
            uint32_t *typedPtr = (uint32_t*)ptr;
            typedPtr[0] = iCol.getRedUint32();
            typedPtr[1] = iCol.getGreenUint32();
            typedPtr[2] = iCol.getBlueUint32();
        }break;
        case iifRgbInt32:
        {
            int32_t *typedPtr = (int32_t*)ptr;
            typedPtr[0] = iCol.getRedInt32();
            typedPtr[1] = iCol.getGreenInt32();
            typedPtr[2] = iCol.getBlueInt32();
        }break;
        case iifRgbF32:
        {
            float *typedPtr = (float*)ptr;
            typedPtr[0] = iCol.getRedF32();
            typedPtr[1] = iCol.getGreenF32();
            typedPtr[2] = iCol.getBlueF32();
        }break;
        case iifRgbaUint8:
        {
            uint8_t *typedPtr = (uint8_t*)ptr;
            typedPtr[0] = iCol.getRedUint8();
            typedPtr[1] = iCol.getGreenUint8();
            typedPtr[2] = iCol.getBlueUint8();
            typedPtr[3] = iCol.getAlphaUint8();
        }break;
        case iifRgbaInt8:
        {
            int8_t *typedPtr = (int8_t*)ptr;
            typedPtr[0] = iCol.getRedInt8();
            typedPtr[1] = iCol.getGreenInt8();
            typedPtr[2] = iCol.getBlueInt8();
            typedPtr[3] = iCol.getAlphaInt8();
        }break;
        case iifRgbaUint16:
        {
            uint16_t *typedPtr = (uint16_t*)ptr;
            typedPtr[0] = iCol.getRedUint16();
            typedPtr[1] = iCol.getGreenUint16();
            typedPtr[2] = iCol.getBlueUint16();
            typedPtr[3] = iCol.getAlphaUint16();
        }break;
        case iifRgbaInt16:
        {
            int16_t *typedPtr = (int16_t*)ptr;
            typedPtr[0] = iCol.getRedInt16();
            typedPtr[1] = iCol.getGreenInt16();
            typedPtr[2] = iCol.getBlueInt16();
            typedPtr[3] = iCol.getAlphaInt16();
        }break;
        case iifRgbaF16:
        {
            half *typedPtr = (half*)ptr;
            typedPtr[0] = iCol.getRedF16();
            typedPtr[1] = iCol.getGreenF16();
            typedPtr[2] = iCol.getBlueF16();
            typedPtr[3] = iCol.getAlphaF16();
        }break;
        case iifRgbaUint32:
        {
            uint32_t *typedPtr = (uint32_t*)ptr;
            typedPtr[0] = iCol.getRedUint32();
            typedPtr[1] = iCol.getGreenUint32();
            typedPtr[2] = iCol.getBlueUint32();
            typedPtr[3] = iCol.getAlphaUint32();
        }break;
        case iifRgbaInt32:
        {
            int32_t *typedPtr = (int32_t*)ptr;
            typedPtr[0] = iCol.getRedInt32();
            typedPtr[1] = iCol.getGreenInt32();
            typedPtr[2] = iCol.getBlueInt32();
            typedPtr[3] = iCol.getAlphaInt32();
        }break;
        case iifRgbaF32:
        {
            float *typedPtr = (float*)ptr;
            typedPtr[0] = iCol.getRedF32();
            typedPtr[1] = iCol.getGreenF32();
            typedPtr[2] = iCol.getBlueF32();
            typedPtr[3] = iCol.getAlphaF32();
        }break;
        default: assert(0); break;
        }
    }
}

//----------------------------------------------------------------------------
void Image::setPixelColor(Math::Vector2 iPixel, const Color& iCol)
{
    assert(0 && "Interpolation needs to be implemented!");
    setPixelColor(Vector2i((int)iPixel.x(), (int)iPixel.y()), iCol);
}

//----------------------------------------------------------------------------
void Image::setWrapType(WrapType iWt)
{
    mWrapType = iWt;
}

//----------------------------------------------------------------------------
// Unloads binary image data but keeps everything else.
//
void Image::unloadImageData()
{
    if (hasImageData())
    {
        mImageData.clear();
    }
}

