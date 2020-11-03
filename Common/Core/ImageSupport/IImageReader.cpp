
#include <cassert>
#include "Core/ImageSupport/IImageReader.h"

using namespace Realisim;
    using namespace Core;

//---------------------------------------------------------------------------------------------------------------------
IImageReader::IImageReader() :
    mImageData(),
    mIsValid(false),
    mFilenamePath(),
    mBytesPerChannel(0),
    mWidthInPixel(0),
    mHeightInPixel(0),
    mNumberOfChannels(0)
{}

//---------------------------------------------------------------------------------------------------------------------
IImageReader::IImageReader(const std::string &iFilenamePath) :
    mImageData(),
    mIsValid(false),
    mFilenamePath(iFilenamePath),
    mBytesPerChannel(0),
    mWidthInPixel(0),
    mHeightInPixel(0),
    mNumberOfChannels(0)
{}

//---------------------------------------------------------------------------------------------------------------------
IImageReader::~IImageReader()
{}

//---------------------------------------------------------------------------------------------------------------------
void IImageReader::clear()
{
    mImageData = ByteArray();
    mIsValid = false;

    mFilenamePath;
    mBytesPerChannel = 0;
    mWidthInPixel = 0;
    mHeightInPixel = 0;
    mNumberOfChannels = 0;
}

//---------------------------------------------------------------------------------------------------------------------
int IImageReader::getBytesPerChannel() const
{
    return mBytesPerChannel;
}

//---------------------------------------------------------------------------------------------------------------------
const std::string& IImageReader::getFilenamePath() const
{
    return mFilenamePath;
}

//---------------------------------------------------------------------------------------------------------------------
int IImageReader::getHeightInPixel() const
{
    return mHeightInPixel;
}

//---------------------------------------------------------------------------------------------------------------------
ByteArray IImageReader::getImageData() const
{
    return mImageData;
}

//---------------------------------------------------------------------------------------------------------------------
ImageInternalFormat IImageReader::getInternalFormat() const
{
    ImageInternalFormat r = iifUndefined;
    switch (getNumberOfChannels())
    {
    case 1:
    {
        switch (getBytesPerChannel())
        {
        case 1: r = iifRUint8; break;
        case 2: r = iifRUint16; break;
        case 3: assert(0); break;
        case 4: r = iifRUint32; break;
        default: assert(0); break;
        }
    }break;
    case 3:
    {
        switch (getBytesPerChannel())
        {
        case 1: r = iifRgbUint8; break;
        case 2: r = iifRgbUint16; break;
        case 3: assert(0); break;
        case 4: r = iifRgbUint32; break;
        default: assert(0); break;
        }
    }break;
    case 4:
    {
        switch (getBytesPerChannel())
        {
        case 1: r = iifRgbaUint8; break;
        case 2: r = iifRgbaUint16; break;
        case 3: assert(0); break;
        case 4: r = iifRgbaUint32; break;
        default: assert(0); break;
        }
    }break;
    default: assert(0); break;
    }

    return r;
}

//---------------------------------------------------------------------------------------------------------------------
int IImageReader::getNumberOfChannels() const
{
    return mNumberOfChannels;
}

//---------------------------------------------------------------------------------------------------------------------
Math::Vector2i IImageReader::getSizeInPixel() const
{
    return Math::Vector2i(mWidthInPixel, mHeightInPixel);
}

//---------------------------------------------------------------------------------------------------------------------
// Returns the size in bytes of the image data, is will not account for header information...
// This method can be called after a call to loadHeader.
//
uint64_t IImageReader::getSizeInBytes() const
{
    return mBytesPerChannel * mWidthInPixel * mHeightInPixel * mNumberOfChannels;
}

//---------------------------------------------------------------------------------------------------------------------
int IImageReader::getWidthInPixel() const
{
    return mWidthInPixel;
}

//---------------------------------------------------------------------------------------------------------------------
bool IImageReader::hasImageData() const
{ return !mImageData.isEmpty(); }

//---------------------------------------------------------------------------------------------------------------------
bool IImageReader::isValid() const
{
    return mIsValid;
}

//---------------------------------------------------------------------------------------------------------------------
void IImageReader::load()
{}

//---------------------------------------------------------------------------------------------------------------------
void IImageReader::loadHeader()
{}

//---------------------------------------------------------------------------------------------------------------------
void IImageReader::setFilenamePath(const std::string& iF)
{
    mFilenamePath = iF;
}
