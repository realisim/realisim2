#include <cassert>
#include "RawImage.h"
#include "Core/StreamUtility.h"

using namespace Realisim;
    using namespace Core;
using namespace std;

namespace
{
    const uint32_t kRawMagicNumber = 0x773F3290;
    const uint16_t kRawVersion = 1;
    const int kHeaderSize = 64;
}

//------------------------------------------------------------------------------
RawImage::RawImage() : IImageReader(),
mInternalFormat(iifUndefined)
{}

//------------------------------------------------------------------------------
RawImage::RawImage(const std::string& iFilenamePath) :
    IImageReader(iFilenamePath),
    mInternalFormat(iifUndefined)
{
    load();
}

//------------------------------------------------------------------------------
RawImage::~RawImage()
{
    clear();
}

//------------------------------------------------------------------------------
void RawImage::clear()
{
    IImageReader::clear();
    mInternalFormat = iifUndefined;
}

//------------------------------------------------------------------------------
ImageInternalFormat RawImage::getInternalFormat() const
{
    return mInternalFormat;
}

//------------------------------------------------------------------------------
void RawImage::load()
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
            bool ok = true;
            Realisim::Core::StreamUtility su;
            su.setStreamFormat(Realisim::Core::StreamUtility::eLittleEndian);

            
            uint64_t sizeOfData;
            ok &= su.readUint64(ifs, &sizeOfData);
            ok &= su.readBytes(ifs, sizeOfData, &mImageData.nonConstString());

            mIsValid = ok;
        }
    }
    ifs.close();
}

//------------------------------------------------------------------------------
void RawImage::loadHeader()
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
bool RawImage::loadHeader(ifstream& ifs)
{
    Realisim::Core::StreamUtility su;
    su.setStreamFormat(Realisim::Core::StreamUtility::eLittleEndian);
    
    //read header
    bool ok = true;
    uint32_t magicNumber;
    ok &= su.readUint32(ifs, &magicNumber);
    ok &= magicNumber == kRawMagicNumber;
    
    uint16_t version;
    ok &= su.readUint16(ifs, &version);

    uint8_t enc;
    ok &= su.readUint8(ifs, &enc);

    if(ok)
    {
       ifs.seekg(kHeaderSize, ios::beg);

        uint32_t w, h;
        uint16_t internalFormat;

        ok &= su.readUint32(ifs, &w);
        ok &= su.readUint32(ifs, &h);
        ok &= su.readUint16(ifs, &internalFormat);
        mWidthInPixel = w;
        mHeightInPixel = h;
        mInternalFormat = (ImageInternalFormat)internalFormat;
        mNumberOfChannels = (int8_t)Core::getNumberOfChannels(getInternalFormat());
    }
  
    return ok;
}

//------------------------------------------------------------------------------
// a kHeaderSize bytes header

// 0    uint32      magic header
// 4    uint32      version
// 8    uint8       encoding 
// ... unused
// 64   uint32      width in pixels
// 68   uint32      height in pixels
// 72   uint16      internal format
// 74   uint64      size of raw data
// 82   begin of raw data.
//
bool RawImage::save(std::string iFilenamePath,
    int iWidth, unsigned int iHeight, 
    ImageInternalFormat iF, 
    const ByteArray iData)
{
    bool r = false;

    ofstream ofs;
    ofs.open(iFilenamePath, ios::out | ios::binary | ios::trunc);
    if (ofs.is_open())
    {
        StreamUtility su;
        su.setStreamFormat(StreamUtility::eLittleEndian);

        su.write(ofs, (uint32_t)kRawMagicNumber);
        su.write(ofs, (uint16_t)kRawVersion);
        su.write(ofs, (uint8_t)eNone);

        //write 0 for the rest of header
        std::streampos currentPos = ofs.tellp();
        const string zeros(kHeaderSize - currentPos, '\0');
        su.writeBytes(ofs, zeros);

        su.write(ofs, (uint32_t)iWidth);
        su.write(ofs, (uint32_t)iHeight);
        su.write(ofs, (uint16_t)iF);
        su.write(ofs, (uint64_t)iData.size());
        su.write(ofs, iData);

        ofs.flush();
        ofs.close();

        r = true;
    }

    return r;
}

////------------------------------------------------------------------------------
//void RawImage::setFilenamePath(const std::string& iV)
//{ mFilenamePath = iV; }


