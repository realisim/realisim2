
#pragma once

#include <Core/ByteArray.h>
#include <Core/ImageInternalFormat.h>
#include <fstream>
#include <stdint.h>

namespace Realisim
{
namespace Core
{

/*------------------------------------------------------------------------------
 
------------------------------------------------------------------------------*/
class RawImage
{
public:
    RawImage();
    RawImage(const std::string& iFilenamePath);
    RawImage(const RawImage&) = default;
    RawImage& operator=(const RawImage&) = default;
    ~RawImage();
    
    void clear();
    const std::string& getFilenamePath() const;
    unsigned int getHeight() const;
    ByteArray getImageData() const;
    ImageInternalFormat getInternalFormat() const;
    unsigned int getWidth() const;
    bool hasImageData() const;
    bool isValid() const;
    void load();
    void loadHeader();
    static bool save(std::string iFilenamePath, int iWidth, unsigned int iHeight, ImageInternalFormat iF, const ByteArray iData);
    void setFilenamePath(const std::string&);
    
protected:
    enum encoding {eNone = 0, eRle};

    bool loadHeader(std::ifstream& ifs);

    std::string mFilenamePath;
    unsigned int mWidth;
    unsigned int mHeight;
    ImageInternalFormat mInternalFormat;
    ByteArray mImageData;
    bool mIsValid;
};

}
}
