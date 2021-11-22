
#include <cassert>
#include "OpenGlHeaders.h"
#include "Rendering/Gpu/Utilities.h"
#include "Rendering/Gpu/Texture3d.h"

using namespace Realisim;
    using namespace Rendering;
    using namespace Math;

//-----------------------------------------------------------------------------
Texture3d::Texture3d() :
    mId(0),
    mNumberOfSamples(1),
	mNumberOfMipmaps(0),
    mMaxAnisotropicFilteringValue(1.0f),
    mTarget(tt2dArray),
    mInternalFormat(tifSrgb8),
    mFormat(tfRgba),
    mDataType(dtUnsignedByte),
    mMagnificationFilter(tfNearest),
    mMinificationFilter(tfNearest),
    mWrapS(twmClampToBorder),
    mWrapT(twmClampToBorder),
    mWrapR(twmClampToBorder),
    mSizeInPixels(0, 0, 0),
    mSizeInBytes(0),
    mBoundTextureUnit(-1),
    mShouldUpdateParameters(true)
{
}

//-----------------------------------------------------------------------------
Texture3d::~Texture3d()
{
    clear();
}

//-----------------------------------------------------------------------------
void Texture3d::bind(unsigned int iTextureUnit /*=0*/)
{
    mBoundTextureUnit = iTextureUnit;
    glActiveTexture(GL_TEXTURE0 + mBoundTextureUnit);

    const GLenum target = toGlTextureTarget(getTarget());
    glBindTexture(target, getId());

    updateTextureParameters();
}

//----------------------------------------------------------------------------
void Texture3d::generateMipmap()
{
    if(getId()>0)
        glGenerateTextureMipmap(getId());
}

//----------------------------------------------------------------------------
// release all OpenGL ressources
//
void Texture3d::clear()
{
    releaseOpenGlResources();

    mNumberOfSamples = 1;
    mTarget = tt2dArray;
    mInternalFormat = tifSrgb8;
    mFormat = tfRgba;
    mDataType = dtUnsignedByte;
    mMagnificationFilter = tfNearest;
    mMinificationFilter = tfNearest;
    mWrapS = twmClampToBorder;
    mWrapT = twmClampToBorder;
    mWrapR = twmClampToBorder;
    mSizeInPixels.set(0, 0, 0);
    mSizeInBytes = 0;
    mBoundTextureUnit = -1;
	mNumberOfMipmaps = 0;
    mMaxAnisotropicFilteringValue = 1.0f;
}

//--------------------------------------------------------------------------
DataType Texture3d::getDataType() const
{ return mDataType; }

//--------------------------------------------------------------------------
TextureFormat Texture3d::getFormat() const
{ return mFormat; }

//--------------------------------------------------------------------------
int Texture3d::getHeight() const
{
    return mSizeInPixels.y();
}

//--------------------------------------------------------------------------
int Texture3d::getDepth() const
{
    return mSizeInPixels.z();
}

//--------------------------------------------------------------------------
int Texture3d::getId() const
{ return mId; }

//--------------------------------------------------------------------------
TextureInternalFormat Texture3d::getInternalFormat() const
{ return mInternalFormat; }

//--------------------------------------------------------------------------
TextureFilter Texture3d::getMagnificationFilter() const
{
    return mMagnificationFilter;
}

//--------------------------------------------------------------------------
TextureFilter Texture3d::getMinificationFilter() const
{
    return mMinificationFilter;
}

//--------------------------------------------------------------------------
double Texture3d::getMaxAnisotropicFilteringValue() const
{
    return mMaxAnisotropicFilteringValue;
}

//--------------------------------------------------------------------------
int Texture3d::getNumberOfChannels() const
{
    int r = 0;
    switch( getFormat() )
    {
        case tfRed: r = 1; break;
        case tfRg: r = 2; break;
        case tfRgb: r = 3; break;
        case tfRgba: r = 4; break;
        case tfStencil: r = 1; break;
        case tfDepth: r = 1; break;
        case tfDepthStencil: r = 1; break;
        default: r = 0; break;
    }
    return r;
}

//--------------------------------------------------------------------------
int Texture3d::getNumberOfMipmaps() const
{ return mNumberOfMipmaps; }

//------------------------------------------------------------------------------
int Texture3d::getNumberOfSamples() const
{
    return mNumberOfSamples;
}

//--------------------------------------------------------------------------
int Texture3d::getSizeInBytes() const
{
    return mSizeInBytes;
}

//--------------------------------------------------------------------------
Math::Vector3i Texture3d::getSizeInPixels() const
{
    return mSizeInPixels;
}

//--------------------------------------------------------------------------
TextureTarget Texture3d::getTarget() const
{
    return mTarget;
}

//--------------------------------------------------------------------------
int Texture3d::getWidth() const
{
    return mSizeInPixels.x(); 
}

//--------------------------------------------------------------------------
TextureWrapMode Texture3d::getWrapSMode() const
{
    return mWrapS;
}

//--------------------------------------------------------------------------
TextureWrapMode Texture3d::getWrapTMode() const
{
    return mWrapT;
}

//--------------------------------------------------------------------------
TextureWrapMode Texture3d::getWrapRMode() const
{
    return mWrapR;
}

//--------------------------------------------------------------------------
bool Texture3d::hasMipmaps() const
{
    return false;
}

//--------------------------------------------------------------------------
bool Texture3d::isCompressed() const
{
    TextureInternalFormat tif = getInternalFormat();

    return
        tif == tifCompressedRgbDxt1 ||
        tif == tifCompressedRgbaDxt1 ||
        tif == tifCompressedRgbaDxt3 ||
        tif == tifCompressedRgbaDxt5 ||
        tif == tifCompressedSrgbDxt1 ||
        tif == tifCompressedSrgbaDxt1 ||
        tif == tifCompressedSrgbaDxt3 ||
        tif == tifCompressedSrgbaDxt5;
}

//--------------------------------------------------------------------------
bool Texture3d::isValid() const
{
    return getId() != 0;
}

//--------------------------------------------------------------------------
void Texture3d::releaseOpenGlResources()
{
    if (getId() != 0)
    {
        glDeleteTextures(1, &mId);
        mId = 0;
    }
}

//-----------------------------------------------------------------------------
// This method assumes that the texture is binded. This method is called
// from bind and works with a dirty flag. see mShouldUpdateParameters
//
void Texture3d::updateTextureParameters()
{
    if (mShouldUpdateParameters && getId())
    {
        mShouldUpdateParameters = false;
        
        const GLenum magFilter = toGlTextureFilter(getMagnificationFilter());
        const GLenum minFilter = toGlTextureFilter(getMinificationFilter());
        const GLenum wrapS = toGlTextureWrapMode(getWrapSMode());
        const GLenum wrapT = toGlTextureWrapMode(getWrapTMode());
        const GLenum wrapR = toGlTextureWrapMode(getWrapRMode());

        glPixelStorei(GL_UNPACK_ALIGNMENT,1); // keep it !

        glTextureParameteri(getId(), GL_TEXTURE_MAG_FILTER, magFilter);
        glTextureParameteri(getId(), GL_TEXTURE_MIN_FILTER, minFilter);
        glTextureParameteri(getId(), GL_TEXTURE_WRAP_S, wrapS);
        glTextureParameteri(getId(), GL_TEXTURE_WRAP_T, wrapT);
        glTextureParameteri(getId(), GL_TEXTURE_WRAP_R, wrapR);

        glTextureParameterf(getId(), GL_TEXTURE_MAX_ANISOTROPY_EXT, mMaxAnisotropicFilteringValue);

        getAndClearLastOpenGlErrors();
    }
}

//--------------------------------------------------------------------------
void Texture3d::initialize(TextureTarget iTarget, 
    TextureInternalFormat iInternalFormat,
    int nbMipmap,
	int iWidth, 
    int iHeight, 
    int iDepth, 
    TextureFormat iFormat, 
    DataType iDataType)
{
    if ((iWidth >= 1) && (iHeight >= 1) && (iDepth >= 1))
    {
        if (getId() != 0)
        {
            releaseOpenGlResources();
        }

        mNumberOfMipmaps = nbMipmap;
        mNumberOfSamples = 1;
        mTarget = iTarget;
        mInternalFormat = iInternalFormat;
        mFormat = iFormat;
        mDataType = iDataType;
        mSizeInPixels.set(iWidth, iHeight, iDepth);
        mSizeInBytes = iWidth * iHeight * iDepth * getNumberOfChannels() * getSizeOf(getDataType());

        const GLenum target = toGlTextureTarget(getTarget());

        glCreateTextures(target, 1, &mId);
        const GLenum internalFormat = toGlTextureInternalFormat(getInternalFormat());

        updateTextureParameters();
        glTextureStorage3D(getId(), mNumberOfMipmaps, internalFormat, getWidth(), getHeight(), getDepth());
    }
}

//--------------------------------------------------------------------------
void Texture3d::set(
    int nMipmapIndex,
    int imageIndex, int iWidth, int iHeight, int iDepth,
    void *ipData)
{
	const GLenum format = toGlTextureFormat(getFormat());
	const GLenum dataType = toGlDataType(getDataType());

    if (ipData)
    {
		updateTextureParameters();
        // now send the image
        glTextureSubImage3D(getId(),
            nMipmapIndex,	// mipmap level
            0, 0, imageIndex, // x,y,z offset
            iWidth, iHeight, iDepth,
            format, dataType, ipData);
    }
}

//--------------------------------------------------------------------------
void Texture3d::setAsCompressed(int nMipmapIndex,
    int imageIndex,
    int iWidth,
    int iHeight,
    int iDepth,
    int iImageSize,
    void *ipData)
{
    GLenum internalFormat = toGlTextureInternalFormat(getInternalFormat());

    if(ipData)
    {
		updateTextureParameters();

        glCompressedTextureSubImage3D(getId(),nMipmapIndex,
            0,0,imageIndex, // offset x,y,z
            iWidth,iHeight,iDepth,
            internalFormat,
            iImageSize,ipData);
    }
}

//------------------------------------------------------------------------------
void Texture3d::setMagnificationFilter(TextureFilter iTf)
{
    mShouldUpdateParameters = true;
    mMagnificationFilter = iTf;
}

//------------------------------------------------------------------------------
void Texture3d::setMaxAnisotropicFilteringValue(double iV)
{
    mShouldUpdateParameters = true;
    mMaxAnisotropicFilteringValue = (float)iV;
}


//------------------------------------------------------------------------------
void Texture3d::setMinificationFilter(TextureFilter iTf)
{
    mShouldUpdateParameters = true;
    mMinificationFilter = iTf;
}

//------------------------------------------------------------------------------
void Texture3d::setWrapSMode(TextureWrapMode iTwm)
{
    mShouldUpdateParameters = true;
    mWrapS = iTwm;
}

//------------------------------------------------------------------------------
void Texture3d::setWrapTMode(TextureWrapMode iTwm)
{
    mShouldUpdateParameters = true;
    mWrapT = iTwm;
}

//------------------------------------------------------------------------------
void Texture3d::setWrapRMode(TextureWrapMode iTwm)
{
    mShouldUpdateParameters = true;
    mWrapR = iTwm;
}

//------------------------------------------------------------------------------
void Texture3d::setWrapMode(TextureWrapMode iTwm)
{
    mShouldUpdateParameters = true;
    mWrapS = iTwm;
    mWrapT = iTwm;
    mWrapR = iTwm;
}
//------------------------------------------------------------------------------
void Texture3d::unbind()
{
    if (mBoundTextureUnit != -1)
    {
        glActiveTexture(GL_TEXTURE0 + mBoundTextureUnit);

        const GLenum target = toGlTextureTarget(getTarget());
        glBindTexture(target, 0);
        mBoundTextureUnit = -1;
    }
}

