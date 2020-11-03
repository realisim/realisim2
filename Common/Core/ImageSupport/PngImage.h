#pragma once
#include "Core/ImageSupport/IImageReader.h"


namespace Realisim
{
namespace Core
{
    class PngImage : public IImageReader
    {
	public:
        PngImage();
        PngImage(const std::string& iFilenamePath);
        PngImage(const PngImage&) = default;
        PngImage& operator=(const PngImage&) = default;
        virtual ~PngImage();
        
        void flipVertical();
        virtual ImageInternalFormat getInternalFormat() const override;
        virtual void load() override;
        void load(bool iFlip);
        virtual void loadHeader() override;
        static bool save(const std::string& iFilenamePath, int iWidth, int iHeight, int iBytesPerChannel, int iNumberOfChannels, const ByteArray iByteArray);
        
    protected:
    };

}
}