

#pragma once

#include "Rendering/Gpu/DataType.h"
#include "Math/VectorI.h"

namespace Realisim
{
namespace Rendering
{
    /*
    This class handles GL resources associated with a texture2d object.

    Simply, image data is pased via set() method.
    Compressed image data (such ass DDS) must be passed via setAsCompressed.

    isValid() will indicate if the GL resources were assigned properly.
    */
    class Texture2d
    {
    public:
        Texture2d();
        ~Texture2d();
        Texture2d(const Texture2d&) = delete;
        Texture2d& operator=(const Texture2d&) = delete;

        //Utilities::ByteArray asBuffer() const;
        void bind(unsigned int iTextureUnit = 0) const;
        void clear();
        void generateMipmap(); //untested yet...
        DataType getDataType() const;
        TextureFormat getFormat() const;
        int getHeight() const;
        int getId() const;
        TextureInternalFormat getInternalFormat() const;
        TextureFilter getMagnificationFilter() const;
        double getMaxAnisotropicFilteringValue() const;
        TextureFilter getMinificationFilter() const;
        int getNumberOfChannels() const;
        int getNumberOfMipmaps() const;
        int getNumberOfSamples() const;
        int getSizeInBytes() const;
        Math::Vector2i getSizeInPixels() const;
        TextureTarget getTarget() const;
        int getWidth() const;
        TextureWrapMode getWrapSMode() const;
        TextureWrapMode getWrapTMode() const;
        bool hasMipmaps() const;
        bool isCompressed() const;
        bool isValid() const;
        void resizeFbo(int iWidth, int iHeight);
        void resizeFbo(const Math::Vector2i& iSize);

        void set(TextureTarget iTarget, int iMipMapIndex, int iNumberOfMipMaps, TextureInternalFormat iInternalFormat,
            int iWidth, int iHeight, TextureFormat iFormat, DataType iDataType, void *ipData);
        void setAsCompressed(TextureTarget iTarget, int iMipmapIndex, int nbMipmap, TextureInternalFormat iInternalFormat,
            int iWidth, int iHeight, int iImageSizeInBytes, void *ipData);
        void setFboTexture(TextureTarget iTarget, TextureInternalFormat iInternalFormat,
            int iWidth, int iHeight, TextureFormat iFormat, DataType iDataType, int iNumberOfSamples);
        void setMagnificationFilter(TextureFilter iTf);
        void setMaxAnisotropicFilteringValue(double iV);
        void setMinificationFilter(TextureFilter iTf);
        void setWrapSMode(TextureWrapMode iTwm);
        void setWrapTMode(TextureWrapMode iTwm);
        void setWrapMode(TextureWrapMode iTwm);
        void unbind() const;

        void   copyTo(Texture2d& output, const int& mipmapIndexSrc, const int& mipmapIndexDst) const;
        
    protected:
        void releaseOpenGlResources();
        void updateParameters() const;

        unsigned int mId;
        bool mIsValid;
        TextureTarget mTarget;
        TextureInternalFormat mInternalFormat;
        TextureFormat mFormat;
        DataType mDataType;
        TextureFilter mMagnificationFilter;
        TextureFilter mMinificationFilter;
        TextureWrapMode mWrapS;
        TextureWrapMode mWrapT;
		int	mNumberOfMipmap;
        int mNumberOfSamples;
        float mMaxAnisotropicFilteringValue;

        Math::Vector2i mSizeInPixels;
        int mSizeInBytes;
        mutable int mBoundTextureUnit;
        mutable bool mShouldUpdateParameters;
    };
}
}