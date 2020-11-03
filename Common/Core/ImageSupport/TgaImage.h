

#pragma once
#include "Core/ImageSupport/IImageReader.h"


namespace Realisim
{
namespace Core
{
    class TgaImage : public IImageReader
    {
	public:
        TgaImage();
        TgaImage(const std::string& iFilenamePath);
        TgaImage(const TgaImage&) = default;
        TgaImage& operator=(const TgaImage&) = default;
        virtual ~TgaImage();
        
        virtual void load() override;
        virtual void loadHeader() override;
        
    protected:
    };

}
}