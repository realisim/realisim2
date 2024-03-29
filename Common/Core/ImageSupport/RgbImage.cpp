#include <cassert>
#include "RgbImage.h"
#if defined(_OPENMP)
#include <omp.h>
#endif
#include "Core/StreamUtility.h"
#include <vector>
#include "Core/Timer.h"

using namespace Realisim;
    using namespace Core;
using namespace std;

RgbImage::RgbImage() : IImageReader(),
mMagicNumber(0),
mStorage(0),
mDimension(0),
mMinumumPixelValue(0),
mMaximumPixelValue(0),
mImageName(),
mColorMapId(0)
{}

//------------------------------------------------------------------------------
RgbImage::RgbImage(const std::string& iFilenamePath) :
    IImageReader(iFilenamePath),
    mMagicNumber(0),
    mStorage(0),
    mDimension(0),
    mMinumumPixelValue(0),
    mMaximumPixelValue(0),
    mImageName(),
    mColorMapId(0)
{
    load();
}

//------------------------------------------------------------------------------
RgbImage::~RgbImage()
{
    clear();
}

//------------------------------------------------------------------------------
void RgbImage::clear()
{
    IImageReader::clear();
    
    mStorage = 0;
    mDimension = 0;
    mMinumumPixelValue = 0;
    mMaximumPixelValue = 0;
    mImageName = string();
    mColorMapId = 0;
}

//------------------------------------------------------------------------------
void RgbImage::decompress(const std::string &iRleData, unsigned char *iDest)
{
    int rleIndex = 0;
    int count = 0;
    unsigned char pixel;
    for (;;) {
        pixel = iRleData[rleIndex++];
        count = (int)(pixel & 0x7F);
        if (!count) {
            return;
        }
        if (pixel & 0x80) {
            while (count--) {
                *iDest++ = iRleData[rleIndex++];
            }
        } else {
            pixel = iRleData[rleIndex++];
            while (count--) {
                *iDest++ = pixel;
            }
        }
    }
}

//------------------------------------------------------------------------------
int RgbImage::getColorMapId() const
{ return mColorMapId; }

//------------------------------------------------------------------------------
int RgbImage::getDimension() const
{ return mDimension; }

//------------------------------------------------------------------------------
std::string RgbImage::getImageName() const
{ return mImageName; }

//------------------------------------------------------------------------------
ImageInternalFormat RgbImage::getInternalFormat() const
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
int RgbImage::getMaximumPixelValue() const
{ return mMaximumPixelValue; }

//------------------------------------------------------------------------------
int RgbImage::getMinumumPixelValue() const
{ return mMinumumPixelValue; }

//------------------------------------------------------------------------------
int RgbImage::getStorage() const
{ return mStorage; }

//------------------------------------------------------------------------------
bool RgbImage::isRleEncoded() const
{ return getStorage() == 1; }

//------------------------------------------------------------------------------
void RgbImage::load()
{
    clear();
    // Parse the file
    ifstream ifs;
    ifs.open(getFilenamePath(), ifstream::in | ios_base::binary);
    if(!ifs.fail())
    {
        mIsValid = loadHeader(ifs);
        
        if(isValid())
        {
            if(!isRleEncoded())
            {
                mIsValid = parseAsVerbatim(ifs);
            }
            else
            {
                mIsValid = parseAsRle(ifs);
                if(!isValid())
                {printf("RgbImageLoader::load() - Error while parsing rle data...\n");}
            }
        }
    }
    ifs.close();
}

//------------------------------------------------------------------------------
void RgbImage::loadHeader()
{
    clear();
    // Parse the file
    ifstream ifs;
    ifs.open(getFilenamePath(), ifstream::in | ios_base::binary);
    if(!ifs.fail())
    {
        mIsValid = loadHeader(ifs);
    }
    ifs.close();
}

//------------------------------------------------------------------------------
bool RgbImage::loadHeader(ifstream& ifs)
{
    // see specification
    Realisim::Core::StreamUtility su;
    su.setStreamFormat(Realisim::Core::StreamUtility::eBigEndian);
    
    bool ok = true;
    ok &= su.readInt16(ifs, &mMagicNumber);
    ok &= mMagicNumber == 474;
    
    if(ok)
    {
        ok &= su.readInt8(ifs, &mStorage);
        
        uint16_t width, height, numberOfChannels;
        ok &= su.readInt8(ifs, &mBytesPerChannel);
        ok &= su.readUint16(ifs, &mDimension);
        ok &= su.readUint16(ifs, &width);
        mWidthInPixel = width;
        ok &= su.readUint16(ifs, &height);
        mHeightInPixel = height;
        ok &= su.readUint16(ifs, &numberOfChannels);
        mNumberOfChannels = (int8_t)numberOfChannels;
        ok &= su.readInt32(ifs, &mMinumumPixelValue);
        ok &= su.readInt32(ifs, &mMaximumPixelValue);
        
        int32_t dummy;
        ok &= su.readInt32(ifs, &dummy);
        
        ok &= su.readChar(ifs, 80, &mImageName);
        ok &= su.readInt32(ifs, &mColorMapId);
    }
    
    ifs.seekg(512); //go to end of header.
    
    // a few assumption and checks if the image was read properly
    if (ok)
    {
        if( getBytesPerChannel() != 1 )
        {
            ok = false;
            clear();
            printf("RgbImageLoader::loadHeader - current SGI RGB format (more than 8 bits per channel) is not supported for image %s.\n", getFilenamePath().c_str() );
        }
    }
    
    return ok;
}

//------------------------------------------------------------------------------
// see documentation (link in h file).
bool RgbImage::parseAsRle(std::ifstream &ifs)
{
    bool ok = true;
    
    Realisim::Core::StreamUtility su;
    su.setStreamFormat(Realisim::Core::StreamUtility::eBigEndian);
    
    const int numChannels = getNumberOfChannels();
    const int sx = getWidthInPixel();
    const int sy = getHeightInPixel();
    
    //--- read offset table
    int32_t* startTable = nullptr;
    int32_t* lenghtTable = nullptr;;
    const int tableLenght = sy * numChannels;
    
    startTable = new int32_t[tableLenght];
    for(int i = 0; i < tableLenght; ++i)
    {
        ok &= su.readInt32(ifs, &startTable[i]);
    }
    
    lenghtTable = new int32_t[tableLenght];
    for(int i = 0; i < tableLenght; ++i)
    {
        ok &= su.readInt32(ifs, &lenghtTable[i]);
    }

    // compute remaining size of the stream
    const std::streampos currentStreamPos = ifs.tellg();
    ifs.seekg(0, std::ios::end);
    const size_t remainingSize = ifs.tellg() - currentStreamPos;
    ifs.seekg(currentStreamPos);
    
    // read all image data at once, it is faster and enables openmp usage.
    // it brings in a twist... the rleOffset from startTable are relative to
    // the file, so we will need to adjust the rleOffset to be relative to
    // the rleRawData buffer. (relativeRleOffset = rleOffset - currentStreamPos)
    // 
    string rleRawData;
    ok &= su.readBytes(ifs, (int)remainingSize, &rleRawData);

    //--- create final buffer and recombine channel buffer into a
    // single rgb/rgba buffer.
    const int finalSize = sx * sy * numChannels;
    mImageData.resize(finalSize);

    //--- For each color channel, decompress each scanline and repack rgb(a) into final buffer
    // see parseAsVerbatim for explanation on packing into final buffer.
    //vector<unsigned char*> channels(numChannels);
    unsigned char* scanline = nullptr;
    int channelIndex = 0;
    //#pragma omp parallel for num_threads(numChannels) private(channelIndex, scanline) shared(ok)
    for(channelIndex = 0; channelIndex < numChannels; ++channelIndex)
    {
        scanline = new unsigned char[sx];
        
        //decompress each scanline of channel n
        for(int row = 0; row < sy && ok; ++row)
        {
            //fetch scanline
            int32_t rleOffset = startTable[row + channelIndex * sy];
            int32_t rleLength = lenghtTable[row + channelIndex * sy];
            
            // grab the rledata for the current scanline
            string rleData = rleRawData.substr(rleOffset - currentStreamPos, rleLength);
            decompress( rleData, &scanline[0] );

            for(int i = 0; i < sx; ++i)
            { mImageData[ (row * sx * numChannels) + (i * numChannels) + channelIndex] = scanline[i]; }
        }

        delete[] scanline;
    }
    
    //cleanup
    delete[] startTable;
    delete[] lenghtTable;
    
    return ok;
}
//------------------------------------------------------------------------------
bool RgbImage::parseAsVerbatim(std::ifstream &ifs)
{
    bool ok = true;
    
    //each channel is stored one after the other... so lets read each of them
    //separately and repack them in rgb or rgba format on the fly.
    Realisim::Core::StreamUtility su;
    su.setStreamFormat(Realisim::Core::StreamUtility::eBigEndian);
        
    //read all data at once, it is faster and enables using openmp
    string rawData;
    ok &= su.readBytes(ifs, mWidthInPixel * mHeightInPixel * mNumberOfChannels, &rawData);

    if (ok)
    {
        const int n = getNumberOfChannels();

        // create final buffer to recombine channel buffer into a
        // single rgb/rgba buffer.
        const int finalSize = getWidthInPixel() * getHeightInPixel() * n;
        mImageData.resize(finalSize);

        int i = 0;
        const int sizeOfChannelInBytes = mWidthInPixel * mHeightInPixel;

        //on channel per thread.
        #pragma omp parallel for num_threads(n) private(i)
        for(i = 0; i < n; ++i)
        {
            for(int j = 0; j < sizeOfChannelInBytes; ++j)
            {
                mImageData[(j * n) + i] = rawData[ i*sizeOfChannelInBytes + j ];
            }
        }

    }
    return ok;
}



