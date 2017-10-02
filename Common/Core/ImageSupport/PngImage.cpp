
#include <cassert>
#include <iostream>
#include "lodepng.h"
#include "PngImage.h"
#include <vector>

using namespace Realisim;
    using namespace Core;
using namespace std;

//------------------------------------------------------------------------------
PngImage::PngImage() :
mFilenamePath(),
mImageData(),
mIsValid(false),
mBytesPerChannel(0),
mWidthInPixel(0),
mHeightInPixel(0),
mNumberOfChannels(0)
{}

//------------------------------------------------------------------------------
PngImage::PngImage(const std::string& iFilenamePath) :
    mFilenamePath(iFilenamePath),
    mImageData(),
    mIsValid(false),
    mBytesPerChannel(0),
    mWidthInPixel(0),
    mHeightInPixel(0),
    mNumberOfChannels(0)
{
    load();
}

//------------------------------------------------------------------------------
PngImage::~PngImage()
{
    clear();
}

//------------------------------------------------------------------------------
void PngImage::clear()
{
    mImageData = ByteArray();
    
    mIsValid = false;
    mBytesPerChannel = 0;
    mWidthInPixel = 0;
    mHeightInPixel = 0;
    mNumberOfChannels = 0;
}

//----------------------------------------------------------------------------
// flips the image 180 deg around the x axis
//
void PngImage::flipVertical()
{
    const int lineSize = getWidthInPixel() * getNumberOfChannels() * getBytesPerChannel();
    uint8_t *pTemp = new uint8_t[lineSize];

    uint8_t *pTop = nullptr, *pBottom = nullptr;
    uint8_t *data = (uint8_t*)mImageData.data();

    const int h = getHeightInPixel();
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

//------------------------------------------------------------------------------
int PngImage::getBytesPerChannel() const
{ return mBytesPerChannel; }

//------------------------------------------------------------------------------
const std::string& PngImage::getFilenamePath() const
{ return mFilenamePath; }

//------------------------------------------------------------------------------
ByteArray PngImage::getImageData() const
{ return mImageData; }

//------------------------------------------------------------------------------
ImageInternalFormat PngImage::getInternalFormat() const
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

//------------------------------------------------------------------------------
int PngImage::getNumberOfChannels() const
{ return mNumberOfChannels; }

//------------------------------------------------------------------------------
int PngImage::getHeightInPixel() const
{ return mHeightInPixel; }

//------------------------------------------------------------------------------
Math::Vector2i PngImage::getSizeInPixel() const
{
	return Math::Vector2i(mWidthInPixel, mHeightInPixel);
}

//------------------------------------------------------------------------------
int PngImage::getSizeInBytes() const
{
    return mBytesPerChannel * mWidthInPixel * mHeightInPixel * mNumberOfChannels;
}

//------------------------------------------------------------------------------
int PngImage::getWidthInPixel() const
{ return mWidthInPixel; }

//------------------------------------------------------------------------------
bool PngImage::hasImageData() const
{ return !mImageData.isEmpty(); }

//------------------------------------------------------------------------------
bool PngImage::isValid() const
{ return mIsValid; }

//------------------------------------------------------------------------------
void PngImage::load(bool iFlip /*=true*/)
{
    clear();

    std::vector<unsigned char> png;
    std::vector<unsigned char> image; //the raw pixels
    lodepng::State state; //optionally customize this one

    // by default png will try to decode the image in an rgba format,
    // see lodepng::state
    //
    unsigned error = lodepng::load_file(png, getFilenamePath()); //load the image file with given filename

    //load the header and customize how we want the raw data.
    unsigned w = 0, h = 0;
    if (!error)
    {
        error = lodepng_inspect( &w, &h, &state, &png[0], png.size());

        state.info_raw.colortype = state.info_png.color.colortype;
        state.info_raw.bitdepth = state.info_png.color.bitdepth;        
    }

    if (!error) error = lodepng::decode(image, w, h, state, png);

    // If there's is no error, grab the bytes...
    if (error == 0)
    {
        //
		mWidthInPixel = w;
		mHeightInPixel = h;

        mImageData.set((char*)(&image[0]), (int)image.size());

        // state.info_png contains info about the decoded png...
        //
        switch (state.info_raw.colortype)
        {
        case LCT_GREY: mNumberOfChannels = 1; break;
        case LCT_RGB: mNumberOfChannels = 3; break;
        case LCT_RGBA: mNumberOfChannels = 4; break;
        default: assert(0); break;
        }

        switch (state.info_raw.bitdepth)
        {
        case 8: mBytesPerChannel = 1; break;
        case 16: mBytesPerChannel = 2; break;
        default: assert(0); break;
        }

        if(iFlip)
        {
            flipVertical();
        }

        mIsValid = true;
    }
    else
    { 
        std::cout << "error " << error << ": " << lodepng_error_text(error) << std::endl;
    }
}

//------------------------------------------------------------------------------
void PngImage::loadHeader()
{
    clear();

	std::vector<unsigned char> png;
	unsigned error = lodepng::load_file(png, getFilenamePath()); //load the image file with given filename
	
	unsigned w = 0, h = 0;
	lodepng::State state; //optionally customize this one
	if (!error)
	{
		error = lodepng_inspect(
			&w, &h,
			&state,
			&png[0], png.size());
	}

	if (!error)
	{
		mWidthInPixel = w;
		mHeightInPixel = h;

		//state.info_png contains info about the decoded png...
		switch (state.info_png.color.colortype)
		{
        case LCT_GREY: mNumberOfChannels = 1; break;
		case LCT_RGB: mNumberOfChannels = 3; break;
		case LCT_RGBA: mNumberOfChannels = 4; break;
		default: assert(0); break;
		}

		switch (state.info_png.color.bitdepth)
		{
		case 8: mBytesPerChannel = 1; break;
		case 16: mBytesPerChannel = 2; break;
		default: assert(0); break;
		}

		mIsValid = !error;
	}
}

//------------------------------------------------------------------------------
bool PngImage::save(const std::string& iFilenamePath,
	int iWidth,
	int iHeight,
	int iBytesPerChannel,
	int iNumberOfChannels,
	const ByteArray iByteArray)
{
	std::vector<unsigned char> png;
	lodepng::State state; //optionally customize this one

	switch (iNumberOfChannels)
	{
	case 1:
		state.info_raw.colortype = LCT_GREY;
		state.info_png.color.colortype = LCT_GREY;
		break;
	case 3: 
	    state.info_raw.colortype = LCT_RGB;
	    state.info_png.color.colortype = LCT_RGB;
	    break;
	case 4: 
	    state.info_raw.colortype = LCT_RGBA;
	    state.info_png.color.colortype = LCT_RGBA;
	    break;
	default: assert(0); break;
	}

	switch (iBytesPerChannel)
	{
	case 1:
	    state.info_raw.bitdepth = 8;
	    break;
	case 2:
	    state.info_raw.bitdepth = 16;
	    break;
    case 4:
        state.info_raw.bitdepth = 32;
        break;
	default: assert(0); break;
	}
    state.encoder.auto_convert = false;

	std::vector<unsigned char> imageData((unsigned char*)iByteArray.constData(), (unsigned char*)iByteArray.constData() + iByteArray.size());

	unsigned error = lodepng::encode(png, imageData, iWidth, iHeight, state);
	if (!error)
	{
	    lodepng::save_file(png, iFilenamePath);
	}

	//if there's an error, display it
	if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;

	return error == 0;
}

//------------------------------------------------------------------------------
void PngImage::setFilenamePath(const std::string& iV)
{ 
    clear();
    mFilenamePath = iV;
}


