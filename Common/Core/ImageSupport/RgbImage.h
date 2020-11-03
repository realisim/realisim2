
#pragma once

#include "Core/ImageSupport/IImageReader.h"
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
class RgbImage : public IImageReader
{
public:
    RgbImage();
    RgbImage(const std::string& iFilenamePath);
    RgbImage(const RgbImage&) = default;
    RgbImage& operator=(const RgbImage&) = default;
    ~RgbImage();
    
    void clear();
    int getColorMapId() const;
    int getDimension() const;
    std::string getImageName() const;
    virtual ImageInternalFormat getInternalFormat() const override;
    int getMaximumPixelValue() const;
    int getMinumumPixelValue() const;
    int getStorage() const;
    bool isRleEncoded() const;
    virtual void load() override;
    virtual void loadHeader() override;
    
protected:
    void decompress(const std::string&, unsigned char*);
    bool loadHeader(std::ifstream&);
    bool parseAsRle(std::ifstream&);
    bool parseAsVerbatim(std::ifstream&);
    
    //-- data read from file.
    int16_t mMagicNumber;
    int8_t mStorage;
    uint16_t mDimension;
    int32_t mMinumumPixelValue;
    int32_t mMaximumPixelValue;
    //4 bytes dummy...
    std::string mImageName;
    int32_t mColorMapId;
    //404 bytes ignored...
};

}
}
