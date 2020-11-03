
// Since Realisim code base uses very high warnings, we deactivate them for
// this Image support since the underlying thirdparty does not respect our standard
#ifdef  _WIN32
    #pragma warning(push, 0)
    #define _SCL_SECURE_NO_WARNINGS
#endif //  _WIN32


#include <cassert>
#include "TgaImage.h"
#include "tga/v4d8df65/tga.hpp"


using namespace Realisim;
    using namespace Core;

//---------------------------------------------------------------------------------------------------------------------
TgaImage::TgaImage() : IImageReader()
{}

//---------------------------------------------------------------------------------------------------------------------
TgaImage::TgaImage(const std::string& iFilenamePath) :
    IImageReader(iFilenamePath)
{}

//---------------------------------------------------------------------------------------------------------------------
TgaImage::~TgaImage()
{}

//---------------------------------------------------------------------------------------------------------------------
void TgaImage::load()
{
    clear();
    tga::TGA tga;
    if (tga.Load(getFilenamePath()))
    {
        mImageData.set((const char*)tga.GetData(), (size_t)tga.GetSize());
        mWidthInPixel = tga.GetWidth();
        mHeightInPixel = tga.GetHeight();

        tga::ImageFormat tgaImageFormat = tga.GetFormat();

        switch (tgaImageFormat)
        {
        case tga::ImageFormat::Monochrome: mNumberOfChannels = 1; break;
        case tga::ImageFormat::RGB: mNumberOfChannels = 3; break;
        case tga::ImageFormat::RGBA: mNumberOfChannels = 4; break;
        case tga::ImageFormat::Undefined: mNumberOfChannels = 0; break;
        default: assert(0);  break;
        }

        mBytesPerChannel = (int8_t) (tga.GetSize() / (mWidthInPixel * mHeightInPixel * mNumberOfChannels));

        mIsValid = true;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TgaImage::loadHeader()
{
    // the current thirdParty TGA does not support loading only the header, so as a cheap workaround
    // we will load the image and then  flush the image data...
    //
    clear();
    tga::TGA tga;
    if (tga.Load(getFilenamePath()))
    {
        mImageData.clear();
    }
}


#ifdef  _WIN32
    #pragma warning(pop)
    #undef _SCL_SECURE_NO_WARNINGS
#endif //  _WIN32

