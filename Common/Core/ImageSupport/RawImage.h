
#pragma once

#include "Core/ImageSupport/IImageReader.h"
#include <fstream>
#include <stdint.h>

namespace Realisim
{
namespace Core
{

/*------------------------------------------------------------------------------
 
------------------------------------------------------------------------------*/
class RawImage : public IImageReader
{
public:
    RawImage();
    RawImage(const std::string& iFilenamePath);
    RawImage(const RawImage&) = default;
    RawImage& operator=(const RawImage&) = default;
    virtual ~RawImage();
    
    virtual void clear() override;
    virtual ImageInternalFormat getInternalFormat() const override;
    virtual void load() override;
    virtual void loadHeader() override;
    static bool save(std::string iFilenamePath, int iWidth, unsigned int iHeight, ImageInternalFormat iF, const ByteArray iData);
    
protected:
    enum encoding {eNone = 0, eRle};

    bool loadHeader(std::ifstream& ifs);

    ImageInternalFormat mInternalFormat;
};

}
}
