

#pragma once

#include "Rendering/Gpu/DataType.h"
#include "Math/VectorI.h"

namespace Realisim
{
namespace Rendering
{
    /*
    This class handles GL resources associated with a texture3d object.
    isValid() will indicate if the GL resources were assigned properly.
    */
    class Texture3d
    {
    public:
        Texture3d();
        ~Texture3d();
        Texture3d(const Texture3d&) = delete;
        Texture3d& operator=(const Texture3d&) = delete;

        void bind(unsigned int iTextureUnit = 0);
        void clear();

        void generateMipmap();
        DataType getDataType() const;
        int getDepth() const;
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
        Math::Vector3i getSizeInPixels() const;
        TextureTarget getTarget() const;
        int getWidth() const;
        TextureWrapMode getWrapSMode() const;
        TextureWrapMode getWrapTMode() const;
        TextureWrapMode getWrapRMode() const;
        bool hasMipmaps() const;
        bool isCompressed() const;
        void initialize(TextureTarget iTarget, TextureInternalFormat iInternalFormat, int nbMipmap,
            int iWidth, int iHeight, int iDepth, TextureFormat iFormat, DataType iDataType);
        bool isValid() const;

        void set(int nMipmapIndex, int imageIndex, int iWidth, int iHeight, int iDepth, void *ipData);		
        void setAsCompressed(int nMipmapIndex, int imageIndex, int iWidth, int iHeight,int iDepth, int iImageSize, void *ipData);

        void setMagnificationFilter(TextureFilter iTf);
        void setMaxAnisotropicFilteringValue(double iV);
        void setMinificationFilter(TextureFilter iTf);
        void setWrapSMode(TextureWrapMode iTwm);
        void setWrapTMode(TextureWrapMode iTwm);
        void setWrapRMode(TextureWrapMode iTwm);
        void setWrapMode(TextureWrapMode iTwm);
        void unbind();

    protected:
        void releaseOpenGlResources();
        void updateTextureParameters();

        unsigned int mId;

        TextureTarget mTarget;
        TextureInternalFormat mInternalFormat;
        TextureFormat mFormat;
        DataType mDataType;

        TextureFilter mMagnificationFilter;
        TextureFilter mMinificationFilter;

        TextureWrapMode mWrapS;
        TextureWrapMode mWrapT;
        TextureWrapMode mWrapR;

		int mNumberOfMipmaps;
        int mNumberOfSamples;
        float mMaxAnisotropicFilteringValue;

        Math::Vector3i mSizeInPixels;
        int mSizeInBytes;
        int mBoundTextureUnit;

        bool mShouldUpdateParameters;
    };
}
}