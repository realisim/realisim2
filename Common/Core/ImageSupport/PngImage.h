#pragma once
#include "Core/ByteArray.h"
#include "ImageInternalFormat.h"
#include "Math/VectorI.h"


namespace Realisim
{
namespace Core
{
    class PngImage
    {
	public:
        PngImage();
        PngImage(const std::string& iFilenamePath);
        PngImage(const PngImage&) = default;
        PngImage& operator=(const PngImage&) = default;
        ~PngImage();
        
        void clear();
        int getBytesPerChannel() const;
        void flipVertical();
        const std::string& getFilenamePath() const;
        ByteArray getImageData() const;
        ImageInternalFormat getInternalFormat() const;
        int getNumberOfChannels() const;
        int getHeightInPixel() const; // getHeightInPixel...
        Math::Vector2i getSizeInPixel() const;
        int getSizeInBytes() const;
        int getWidthInPixel() const; // renomer a getWidthInPixel...
        bool hasImageData() const;
        bool isValid() const;
        void load(bool iFlip = true);
        void loadHeader();
        static bool save(const std::string& iFilenamePath, int iWidth, int iHeight, int iBytesPerChannel, int iNumberOfChannels, const ByteArray iByteArray);
        void setFilenamePath(const std::string&);
        
    protected:
        std::string mFilenamePath;
        ByteArray mImageData;
        bool mIsValid;
        
        //-- data read from file.
        int8_t mBytesPerChannel;
        unsigned int mWidthInPixel;
        unsigned int mHeightInPixel;
        int8_t mNumberOfChannels;
    };

}
}