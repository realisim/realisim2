
#pragma once

#include "ByteArray.h"
#include "Color.h"
#include "ImageSupport/ImageInternalFormat.h"
//#include "ImageSupport/DdsImage.h"
#include "ImageSupport/HgtImage.h"
#include "ImageSupport/PngImage.h"
#include "ImageSupport/RawImage.h"
#include "ImageSupport/RgbImage.h"
//#include "ImageSupport/TiffImage.h"
#include "Math/Vector.h"
#include "Math/VectorI.h"

namespace Realisim
{
namespace Core
{
    class HgtImage;
    
    /*-------------------------------------------------------------------------
    This class presents a 2d image.

    2d images comes in many different encoding (format). We support a few, which
    are defined by the enum Format. Details on the formats can be found in
    ImageSupport/.

    Image data is always uncompressed, meaning specific pixel can be manipulated
    by pixel index.

    Image are initialized by the constructor or set() method.
    Ex:
        Image im("somePath");
        im.load(fPng);

        Image im2;
        im2.set("somePath");
        im2.load(fTiff);

        Image im3("somepath/image.png");
        im3.load(); // file format will be deduced from extension.

        Image im4;
        im3.setData(w, h, iifRgbUint8, pointerToData);
        im3.load();
        
    
    Image usually have a binary paylaod which must be explicitly loaded via load() function.
    It is possible to gather all metadata on image by calling loadHeader(). Metadata of
    images are width, height, sizeInbytes, number of channels, bytes per channel
    and internal format. In brief, its everything except the binary payload.

    Once load() or loadHeader() is called, method isValid() will return true when the image
    was properly loaded. It will return false if the metadata or binary paylaod could not be
    read.

    Image data can be in many form. It can be a single channel of bytes up to four channel
    of float 32 bits. (see ImageInternalFormat).
    The internalFormat is available through getInternalFormat();

    Notes on Supported Format:
        fRaw: This is the in-house format, it means that data is in raw form.
        fRgb: SGI rgb.
        fDds: DDS
        fPng:
        fTiff:
        fHgt:
    
    Saving images:
        

    Explanation on origin of pixel at (0.5, 0.5)
    Explanantion on interpolation set to clamp to border

    Note:
        The image class has dependency on many files, somes are made in house and others
        are thirdparty.

        In-house:
            ImageSupport/DdsImage.h - requires squish.lib to uncompress dds. requires nvvt to compress and save dds to disk.
            ImageSupport/RgbImage.h
            ImageSupport/HgtImage.h (height map with SRTM data).

        ThirdParty:
            LodePng (see CMakeModules/Lodepng.cmake and ImageSupport/PngImage.h)
            tiff (see CMakeModules/LibTiff.cmake, CMakeModules/LibGeoTiff.cmake and ImageSupport/TiffImage.h)
    */
    class Image
    {
    public:
        enum Format { fUnsupported, fRaw, fRgb, /*fDds,*/ fPng, /*fTiff,*/ fHgt }; // renomer a FileFormat

        enum WritableFormat { wfPng, wfRaw/*, wfDds*/ };

        Image();
        Image(const std::string& iFilenamePath);
        Image(const Image&) = default;
        Image& operator=(const Image&) = default;
        virtual ~Image();

        virtual void clear();
        virtual void flipVertical(); //flips 180 deg around x axis
        virtual int getBytesPerChannel() const;
        static std::string getExtensionFromFormat(Format);
        virtual std::string getFilenamePath() const;        
        static Format getFormatFromExtension(const std::string& iExtension);
        static Format getFormatFromFilename(const std::string& iFilename);
        virtual int getHeight() const;
        virtual ImageInternalFormat getInternalFormat() const;
        virtual ByteArray getImageData() const;
        virtual int getNumberOfChannels() const;
        virtual Color getPixelColor(Math::Vector2i iPixel) const;
        virtual Color getPixelColor(Math::Vector2 iPixel) const;
        virtual uint64_t getSizeInBytes() const;
        virtual const Math::Vector2i& getSizeInPixels() const;
        virtual int getWidth() const;
        virtual bool hasImageData() const;
        virtual bool isValid() const;
        virtual bool load();
        virtual bool load(Format);
        virtual bool loadHeader();
        virtual bool loadHeader(Format);
        virtual bool saveAs(const std::string& iFilenamePath, WritableFormat iF);
        //virtual bool saveAs(const std::string& iFilenamePath, const DdsImage::SaveOptions& iSaveOption); //implicitly save as DDS.
        virtual void set(const std::string& iFilenamePath);
        virtual void setFilenamePath(const std::string& iFilenamePath);
        virtual void set(int iWidth, int iHeight, ImageInternalFormat iIf);
        virtual void set(const Math::Vector2i& iSize, ImageInternalFormat iIf);
        virtual void setData(int iWidth, int iHeight, ImageInternalFormat iIf, const char* ipData);

        virtual void setPixelColor(Math::Vector2i iPixel, const Color& iCol); //this should be changed to Color which abstracts rgb8, 16 and float
        virtual void setPixelColor(Math::Vector2 iPixel, const Color& iCol);
        virtual void unloadImageData();
        
    protected:
        virtual void flipVertical(ByteArray& iBa);
        virtual Format guessFormatFromFileName() const;
        //virtual bool loadDdsImage(const std::string& iFilenamePath, bool iHeaderOnly);
        virtual bool loadHgtImage(const std::string& iFilenamePath, bool iHeaderOnly);
        virtual bool loadPngImage(const std::string& iFilenamePath, bool iHeaderOnly);
        virtual bool loadRawImage(const std::string& iFilenamePath, bool iHeaderOnly);
        virtual bool loadRgbImage(const std::string& iFilenamePath, bool iHeaderOnly);
        //virtual bool loadTiffImage(const std::string& iFilenamePath, bool iHeaderOnly);
        //virtual bool saveDdsImage(const std::string& iFilenamePath, const DdsImage::SaveOptions& iSaveOption);
        virtual bool savePngImage(const std::string& iFilenamePath);
        virtual bool saveRawImage(const std::string& iFilenamePath);

        std::string mFilenamePath;
        ByteArray mImageData;
        Math::Vector2i mSizeInPixel;
        uint64_t mSizeInBytes;
        ImageInternalFormat mInternalFormat;
        bool mIsValid;
    };
}
}