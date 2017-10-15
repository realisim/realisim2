
#include <algorithm>
#include <cassert>
#include "FileInfo.h"
#include "Half/half.hpp"
#include "Image.h"

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

#ifdef _WIN32
    if (_stricmp(iExtension.c_str(), "raw") == 0)
    { r = fRaw; }
    else if (_stricmp(iExtension.c_str(), "rgb") == 0)
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
    else { r = fUnsupported; }
#else
    if(strcasecmp( iExtension.c_str(), "raw" ) == 0)
    { r = fRaw; }
    else if(strcasecmp( iExtension.c_str(), "rgb" ) == 0)
    { r = fRgb; }
//    else if (strcasecmp(iExtension.c_str(), "dds") == 0)
//    { r = fDds; }
    else if (strcasecmp(iExtension.c_str(), "png") == 0)
    { r = fPng; }
//    else if (strcasecmp(iExtension.c_str(), "tif") == 0 ||
//        strcasecmp(iExtension.c_str(), "tiff") == 0)
//    { r = fTiff; }
    else if (strcasecmp(iExtension.c_str(), "hgt") == 0)
    {
        r = fHgt;
    }
    else { r = fUnsupported; }
#endif


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
// No interpolation is happening
//
Color Image::getPixelColor(Math::Vector2i iPixel) const
{    
    Color r;

    if (isValid())
    {
        // here, we clamp to border.
        // py and px are uint64_t to enable the byteOffset on the following
        // line to be truly a uint64 thus enabling larger than 4Gb images..
        //
        const uint64_t px = min(max(0, iPixel.x()), getWidth() - 1);
        const uint64_t py = min(max(0, iPixel.y()), getHeight() - 1);

        const uint64_t byteOffset = ((py * getWidth()) + px) * getBytesPerChannel() * getNumberOfChannels();
       
        const char *pRed = mImageData.constData() + byteOffset;

        switch (getInternalFormat())
        {
        case iifUndefined: break;
        case iifRUint8: r.setRedUint8(*((uint8_t*)pRed)); break;
        case iifRInt8: r.setRedInt8(*pRed); break;
        case iifRUint16: r.setRedUint16(*((uint16_t*)pRed)); break;
        case iifRInt16: r.setRedInt16(*((int16_t*)pRed)); break;
        case iifRF16: r.setRedF16(*((half*)pRed)); break;
        case iifRUint32: r.setRedUint32(*((uint32_t*)pRed)); break;
        case iifRInt32: r.setRedInt32(*((uint32_t*)pRed)); break;
        case iifRF32: r.setRedF32(*((float*)pRed));  break;
        case iifRgbUint8:
        {
            uint8_t *typedPtr = (uint8_t*)pRed;
            r.setUint8(typedPtr[0], typedPtr[1], typedPtr[2]);
        }break;
        case iifRgbInt8:
        {
            int8_t *typedPtr = (int8_t*)pRed;
            r.setInt8(typedPtr[0], typedPtr[1], typedPtr[2]);
        }break;
        case iifRgbUint16:
        {
            uint16_t *typedPtr = (uint16_t*)pRed;
            r.setUint16(typedPtr[0], typedPtr[1], typedPtr[2]);
        }break;
        case iifRgbInt16:
        {
            int16_t *typedPtr = (int16_t*)pRed;
            r.setInt16(typedPtr[0], typedPtr[1], typedPtr[2]);
        }break;
        case iifRgbF16:
        {
            half *typedPtr = (half*)pRed;
            r.setF16(typedPtr[0], typedPtr[1], typedPtr[2]);
        }break;
        case iifRgbUint32:
        {
            uint32_t *typedPtr = (uint32_t*)pRed;
            r.setUint32(typedPtr[0], typedPtr[1], typedPtr[2]);
        }break;
        case iifRgbInt32:
        {
            int32_t *typedPtr = (int32_t*)pRed;
            r.setInt32(typedPtr[0], typedPtr[1], typedPtr[2]);
        }break;
        case iifRgbF32:
        {
            float *typedPtr = (float*)pRed;
            r.setF32(typedPtr[0], typedPtr[1], typedPtr[2]);
        }break;
        case iifRgbaUint8:
        {
            uint8_t *typedPtr = (uint8_t*)pRed;
            r.setUint8(typedPtr[0], typedPtr[1], typedPtr[2], typedPtr[3]);
        }break;
        case iifRgbaInt8:
        {
            int8_t *typedPtr = (int8_t*)pRed;
            r.setInt8(typedPtr[0], typedPtr[1], typedPtr[2], typedPtr[3]);
        }break;
        case iifRgbaUint16:
        {
            uint16_t *typedPtr = (uint16_t*)pRed;
            r.setUint16(typedPtr[0], typedPtr[1], typedPtr[2], typedPtr[3]);
        }break;
        case iifRgbaInt16:
        {
            int16_t *typedPtr = (int16_t*)pRed;
            r.setInt16(typedPtr[0], typedPtr[1], typedPtr[2], typedPtr[3]);
        }break;
        case iifRgbaF16:
        {
            half *typedPtr = (half*)pRed;
            r.setF16(typedPtr[0], typedPtr[1], typedPtr[2], typedPtr[3]);
        }break;
        case iifRgbaUint32:
        {
            uint32_t *typedPtr = (uint32_t*)pRed;
            r.setUint32(typedPtr[0], typedPtr[1], typedPtr[2], typedPtr[3]);
        }break;
        case iifRgbaInt32:
        {
            int32_t *typedPtr = (int32_t*)pRed;
            r.setInt32(typedPtr[0], typedPtr[1], typedPtr[2], typedPtr[3]);
        }break;
        case iifRgbaF32:
        {
            float *typedPtr = (float*)pRed;
            r.setF32(typedPtr[0], typedPtr[1], typedPtr[2], typedPtr[3]);
        }break;
        default: assert(0); break;
        }
        
        
    }
    return r;
}

//----------------------------------------------------------------------------
// see getPixelColor(Math::Vector2i iPixel) const
// The difference is that it performs a bilinear interpolation
//
// see https://en.wikipedia.org/wiki/Bilinear_interpolation
//
Color Image::getPixelColor(Math::Vector2 iPixel) const
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


    //define in which quadrant (from he center of the pixel)
    //the current sample iPixel is and make the 4 color samples
    // required for the bilinear interpolation
    //
    const Vector2i pixelCenterI((int)iPixel.x(), (int)iPixel.y());
    const Vector2 pixelCenter(pixelCenterI.x() + 0.5, pixelCenterI.y() + 0.5);
    const Vector2 d = iPixel - pixelCenter;
    Vector2 p = Vector2(0.0); //point of interpolation in samples normalized space

    //left side
    if (d.x() <= 0)
    {
        //lower left
        if (d.y() <= 0)
        {
            pixelColors[0] = getPixelColor(Vector2i(pixelCenterI.x() - 1, pixelCenterI.y() - 1));
            pixelColors[1] = getPixelColor(Vector2i(pixelCenterI.x() - 0, pixelCenterI.y() - 1));
            pixelColors[2] = getPixelColor(Vector2i(pixelCenterI.x() - 0, pixelCenterI.y() + 0));
            pixelColors[3] = getPixelColor(Vector2i(pixelCenterI.x() - 1, pixelCenterI.y() + 0));
            p = Vector2(1) + d;
        }
        //upper left
        else if (d.y() > 0)
        {
            pixelColors[0] = getPixelColor(Vector2i(pixelCenterI.x() - 1, pixelCenterI.y() - 0));
            pixelColors[1] = getPixelColor(Vector2i(pixelCenterI.x() - 0, pixelCenterI.y() - 0));
            pixelColors[2] = getPixelColor(Vector2i(pixelCenterI.x() - 0, pixelCenterI.y() + 1));
            pixelColors[3] = getPixelColor(Vector2i(pixelCenterI.x() - 1, pixelCenterI.y() + 1));
            p = Vector2(1, 0) + d;
        }
    }
    //right side
    else
    {
        //lower rigth (includes y == 0)!!!
        if (d.y() <= 0)
        {
            pixelColors[0] = getPixelColor(Vector2i(pixelCenterI.x() - 0, pixelCenterI.y() - 1));
            pixelColors[1] = getPixelColor(Vector2i(pixelCenterI.x() + 1, pixelCenterI.y() - 1));
            pixelColors[2] = getPixelColor(Vector2i(pixelCenterI.x() + 1, pixelCenterI.y() + 0));
            pixelColors[3] = getPixelColor(Vector2i(pixelCenterI.x() - 0, pixelCenterI.y() + 0));
            p = Vector2(0, 1) + d;
        }
        // upper right
        else if (d.y() > 0)
        {
            pixelColors[0] = getPixelColor(Vector2i(pixelCenterI.x() - 0, pixelCenterI.y() - 0));
            pixelColors[1] = getPixelColor(Vector2i(pixelCenterI.x() + 1, pixelCenterI.y() + 0));
            pixelColors[2] = getPixelColor(Vector2i(pixelCenterI.x() + 1, pixelCenterI.y() + 1));
            pixelColors[3] = getPixelColor(Vector2i(pixelCenterI.x() + 0, pixelCenterI.y() + 1));
            p = Vector2(0) + d;
        }       
    }
    
    // by construction the sampling space has a width and length of 1.
    // so to normalize, there is no need to divide by the computed length, we know
    // it is one.
    //
    // So technically, the code would be:
    // const double fx0 = (pixels[1].x() - p.x()) / (pixels[1].x() - pixels[0].x());
    // const double fx1 = (p.x() - pixels[0].x()) / (pixels[1].x() - pixels[0].x());
    // const double fy0 = (pixels[3].y() - p.y()) / (pixels[3].y() - pixels[0].y());
    // const double fy1 = (p.y() - pixels[0].y()) / (pixels[3].y() - pixels[0].y());
    //
    // but we can remove the division, since it divides by 1
    const double fx0 = (pixels[1].x() - p.x());
    const double fx1 = (p.x() - pixels[0].x());
    const double fy0 = (pixels[3].y() - p.y());
    const double fy1 = (p.y() - pixels[0].y());

    Color fxy0(
        (fx0*pixelColors[0].getRed()) + (fx1*pixelColors[1].getRed()),
        (fx0*pixelColors[0].getBlue()) + (fx1*pixelColors[1].getBlue()),
        (fx0*pixelColors[0].getGreen()) + (fx1*pixelColors[1].getGreen()),
        (fx0*pixelColors[0].getAlpha()) + (fx1*pixelColors[1].getAlpha())) ;
    
    Color fxy1(
        (fx0*pixelColors[3].getRed()) + (fx1*pixelColors[2].getRed()),
        (fx0*pixelColors[3].getBlue()) + (fx1*pixelColors[2].getBlue()),
        (fx0*pixelColors[3].getGreen()) + (fx1*pixelColors[2].getGreen()),
        (fx0*pixelColors[3].getAlpha()) + (fx1*pixelColors[2].getAlpha()));

    return Color(
        (fy0 * fxy0.getRed()) + (fy1 * fxy1.getRed()),
        (fy0 * fxy0.getBlue()) + (fy1 * fxy1.getBlue()),
        (fy0 * fxy0.getGreen()) + (fy1 * fxy1.getGreen()),
        (fy0 * fxy0.getAlpha()) + (fy1 * fxy1.getAlpha()));
}

//----------------------------------------------------------------------------
uint64_t Image::getSizeInBytes() const
{
    return mSizeInBytes;
}

//----------------------------------------------------------------------------
Math::Vector2i Image::getSizeInPixels() const
{
    return mSizeInPixel;
}

//----------------------------------------------------------------------------
int Image::getWidth() const
{
    return mSizeInPixel.x();
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
        r = load(guessFormatFromFileName());
    }
    return r;
}

//----------------------------------------------------------------------------
bool Image::load(Format iFormat)
{
    bool r = false;
    
    clear();

    switch (iFormat)
    {
	case fUnsupported: assert(0); break;
    case fRaw: r = loadRawImage(getFilenamePath(), false); break;
    case fRgb: r = loadRgbImage(getFilenamePath(), false); break;
//    case fDds: r = loadDdsImage(getFilenamePath(), false); break;
    case fPng: r = loadPngImage(getFilenamePath(), false); break;
//	case fTiff: r = loadTiffImage(getFilenamePath(), false); break;
    case fHgt: r = loadHgtImage(getFilenamePath(), false); break;
    default: assert(0); break;
    }
    mIsValid = r;
    return r;
}

//----------------------------------------------------------------------------
//bool Image::loadDdsImage(const std::string& iFilenamePath, bool iHeaderOnly)
//{
//    bool r = false;
//    DdsImage im;
//	im.setFilenamePath(iFilenamePath);
//	iHeaderOnly ? im.loadHeader() : im.load();
//
//    if (im.isValid())
//    {
//        mFilenamePath = iFilenamePath;
//        mImageData = im.getUncompressedMipmap(0);
//        mSizeInPixel.set(im.getMipmapWidth(0), im.getMipmapHeight(0));
//        mInternalFormat = im.getInternalFormat();
//        mSizeInBytes = im.getMipmapUncompressedSizeInBytes(0);
//
//        r = true;
//    }
//    return r;
//}

//----------------------------------------------------------------------------
bool Image::loadHeader()
{ 
    bool r = false;
    clear();
    if(!getFilenamePath().empty())
    {
        r = loadHeader(guessFormatFromFileName());
    }
    return r;
}

//----------------------------------------------------------------------------
bool Image::loadHeader(Format iFormat)
{
	bool r = false;	

    clear();

	switch (iFormat)
	{
	case fUnsupported: assert(0); break;
    case fRaw: r = loadRawImage(getFilenamePath(), true); break;
	case fRgb: r = loadRgbImage(getFilenamePath(), true); break;
//	case fDds: r = loadDdsImage(getFilenamePath(), true); break;
	case fPng: r = loadPngImage(getFilenamePath(), true); break;
//	case fTiff: r = loadTiffImage(getFilenamePath(), true); break;
    case fHgt: r = loadHgtImage(getFilenamePath(), true); break;
	default: assert(0); break;
	}
	mIsValid = r;
	return r;
}

//----------------------------------------------------------------------------
bool Image::loadHgtImage(const std::string& iFilenamePath, bool iHeaderOnly)
{
    bool r = false;
    HgtImage im;
    im.setFilenamePath(iFilenamePath);
    iHeaderOnly ? im.loadHeader() : im.load();

    if (im.isValid())
    {
        mFilenamePath = iFilenamePath;
        mImageData = im.getImageData();
        mSizeInPixel = im.getSizeInPixel();
        mInternalFormat = im.getInternalFormat();
        mSizeInBytes = im.getSizeInBytes();

        r = true;
    }
    return r;
}

//----------------------------------------------------------------------------
// png support was taken from https://github.com/lvandeve/lodepng/
//
bool Image::loadPngImage(const std::string& iFilenamePath, bool iHeaderOnly)
{
    bool r = false;
    PngImage im;
    im.setFilenamePath(iFilenamePath);
    iHeaderOnly ? im.loadHeader() : im.load();

    if (im.isValid())
    {
        mFilenamePath = iFilenamePath;
        mImageData = im.getImageData();
		mSizeInPixel = im.getSizeInPixel();
        mInternalFormat = im.getInternalFormat();
		mSizeInBytes = im.getSizeInBytes();

        r = true;
    }
    return r;
}

//----------------------------------------------------------------------------
bool Image::loadRawImage(const std::string& iFilenamePath, bool iHeaderOnly)
{
    bool r = false;
    RawImage im;
    im.setFilenamePath(iFilenamePath);

    iHeaderOnly ? im.loadHeader() : im.load();

    if (im.isValid())
    {
        mFilenamePath = iFilenamePath;
        mImageData = im.getImageData();
        mSizeInPixel.set(im.getWidth(), im.getHeight());
        mInternalFormat = im.getInternalFormat();
        mSizeInBytes = getWidth() * getHeight() * getNumberOfChannels() * getBytesPerChannel();

        r = true;
    }
    return r;
}

//----------------------------------------------------------------------------
bool Image::loadRgbImage(const std::string& iFilenamePath, bool iHeaderOnly)
{
    bool r = false;
    RgbImage im;
    im.setFilenamePath(iFilenamePath);

	iHeaderOnly ? im.loadHeader() : im.load();

    if (im.isValid())
    {
        mFilenamePath = iFilenamePath;
        mImageData = im.getImageData();
        mSizeInPixel.set(im.getPixelSizeX(), im.getPixelSizeY());
        mInternalFormat = im.getInternalFormat();
        mSizeInBytes = getWidth() * getHeight() * getNumberOfChannels() * getBytesPerChannel();

        r = true;
    }
    return r;
}

//----------------------------------------------------------------------------
//bool Image::loadTiffImage(const std::string& iFilenamePath, bool iHeaderOnly)
//{
//    bool r = false;
//    TiffImage im;
//    im.setFilenamePath(iFilenamePath);
//
//    iHeaderOnly ? im.loadHeader() : im.load();
//
//    if (im.isValid())
//    {
//        mFilenamePath = iFilenamePath;
//        mImageData = im.getImageData();
//        mSizeInPixel.set(im.getWidthInPixel(), im.getHeightInPixel());
//        mInternalFormat = im.getInternalFormat();
//        mSizeInBytes = im.getSizeInBytes();
//        r = true;
//    }
//    return r;
//}

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
// This method will saveAs dds with specified options see 
// DdsImage::DdsSaveOptions
//
//bool Image::saveAs(const std::string& iFilenamePath, const DdsImage::SaveOptions& iSaveOption)
//{
//    bool r = false;
//    mFilenamePath = iFilenamePath;
//
//    r = saveDdsImage(iFilenamePath, iSaveOption); //default options...
//    return r;
//}
//
////----------------------------------------------------------------------------
//bool Image::saveDdsImage(const std::string& iFilenamePath, const DdsImage::SaveOptions& iSaveOption)
//{
//    //increment refCount on imagedata. So if flip is necessary, we wont modify the original data.
//    //
//    ByteArray ba = getImageData();
//
//    flipVertical(ba);
//
//    return DdsImage::save(iFilenamePath,
//        getWidth(),
//        getHeight(),
//        getInternalFormat(),
//        iSaveOption,
//        ba);
//}

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
    mImageData.set(ipData, getSizeInBytes());

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
// Unloads binary image data but keeps everything else.
//
void Image::unloadImageData()
{
    if (hasImageData())
    {
        mImageData.clear();
    }
}

