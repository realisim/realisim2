
#pragma once

#include "Core/ByteArray.h"
#include "Core/ImageInternalFormat.h"
#include "Math/VectorI.h"
#include <stdint.h>
#include <string>

namespace Realisim
{
namespace Core
{

class IImageReader
{
    public:
        IImageReader();
        IImageReader(const std::string &iFilenamePath);
        IImageReader(const IImageReader&) = default;
        IImageReader& operator=(const IImageReader&) = default;
        virtual ~IImageReader() = 0;

        virtual void clear();
        virtual int getBytesPerChannel() const;
        virtual const std::string& getFilenamePath() const;
        virtual int getHeightInPixel() const;
        virtual ByteArray getImageData() const;
        virtual ImageInternalFormat getInternalFormat() const;
        virtual int getNumberOfChannels() const;
        virtual Math::Vector2i getSizeInPixel() const;
        virtual uint64_t getSizeInBytes() const;
        virtual int getWidthInPixel() const;
        virtual bool hasImageData() const;
        virtual bool isValid() const;
        virtual void load() = 0;
        virtual void loadHeader() = 0;
        virtual void setFilenamePath(const std::string&);

    protected:
        ByteArray mImageData;
        bool mIsValid;

        std::string mFilenamePath;
        int8_t mBytesPerChannel;
        unsigned int mWidthInPixel;
        unsigned int mHeightInPixel;
        int8_t mNumberOfChannels;
};

}
}