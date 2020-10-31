
#pragma once

#include <Core/ByteArray.h>
#include <Core/ImageInternalFormat.h>
#include <fstream>
#include <stdint.h>
#include <string>

namespace Realisim
{
namespace Core
{

/*------------------------------------------------------------------------------
 
 see specification:
    ftp://ftp.sgi.com/graphics/grafica/sgiimage.html
 
 Implementation notes
 
 Implementation of both RLE and VERBATIM format for images with BPC of 1 is 
 required since the great majority of SGI images are in this format. Support for
 images with a 2 BPC is encouraged. If the ZSIZE of an image is 1, it is assumed
 to represent B/W values. If the ZSIZE is 3, it is assumed to represent RGB 
 data, and if ZSIZE is 4, it is assumed to contain RGB data with alpha. The 
 origin for all SGI images is the lower left hand corner. The first scanline 
 (row 0) is always the bottom row of the image.
------------------------------------------------------------------------------*/
class RgbImage
{
public:
    RgbImage();
    RgbImage(const std::string& iFilenamePath);
    RgbImage(const RgbImage&) = default;
    RgbImage& operator=(const RgbImage&) = default;
    ~RgbImage();
    
    void clear();
    int getBytesPerChannel() const;
    int getColorMapId() const;
    int getDimension() const;
    const std::string& getFilenamePath() const;
    ByteArray getImageData() const;
    std::string getImageName() const;
    ImageInternalFormat getInternalFormat() const;
    int getMaximumPixelValue() const;
    int getMinumumPixelValue() const;
    int getNumberOfChannels() const;
    int getPixelSizeX() const;
    int getPixelSizeY() const;
    int getStorage() const;
    bool hasImageData() const;
    bool isRleEncoded() const;
    bool isValid() const;
    void load();
    void loadHeader();
    void setFilenamePath(const std::string&);
    
protected:
    void decompress(const std::string&, unsigned char*);
    bool loadHeader(std::ifstream&);
    bool parseAsRle(std::ifstream&);
    bool parseAsVerbatim(std::ifstream&);
    
    std::string mFilenamePath;
    ByteArray mImageData;
    bool mIsValid;
    
    //-- data read from file.
    int16_t mMagicNumber;
    int8_t mStorage;
    int8_t mBytesPerPixel; // on dirait que c'est mal nommé... plutot bytesPerChannel (voir methode getBytesPerChannel)
    uint16_t mDimension;
    uint16_t mPixelSizeX;
    uint16_t mPixelSizeY;
    uint16_t mNumberOfChannels;
    int32_t mMinumumPixelValue;
    int32_t mMaximumPixelValue;
    //4 bytes dummy...
    std::string mImageName;
    int32_t mColorMapId;
    //404 bytes ignored...
};

}
}
