
#include "Math/CommonMath.h"
#include "Rendering/Gpu/OpenGlHeaders.h"
#include "Rendering/Gpu/Texture2d.h"
#include "Rendering/Gpu/Utilities.h"

using namespace Realisim;
    using namespace Rendering;
    using namespace Math;

//-----------------------------------------------------------------------------
Texture2d::Texture2d() :
    mId(0),	
    mIsValid(false),
    mTarget(tt2d),
    mInternalFormat(tifSrgb8),
    mFormat(tfRgba),
    mDataType(dtUnsignedByte),
    mMagnificationFilter(tfNearest),
    mMinificationFilter(tfNearest),
    mWrapS(twmClampToBorder),
    mWrapT(twmClampToBorder),
    mNumberOfMipmap(0),
    mNumberOfSamples(1),
    mMaxAnisotropicFilteringValue(1.0f),
    mSizeInPixels(0, 0),
    mSizeInBytes(0),
    mBoundTextureUnit(-1),
    mShouldUpdateParameters(true)
{}

//-----------------------------------------------------------------------------
Texture2d::~Texture2d()
{
    clear();
}

//-----------------------------------------------------------------------------
void Texture2d::bind(unsigned int iTextureUnit /*=0*/) const
{
    mBoundTextureUnit = iTextureUnit;
    glActiveTexture(GL_TEXTURE0 + mBoundTextureUnit);

    const GLenum target = toGlTextureTarget(getTarget());
    glBindTexture(target, getId());

    updateParameters();
}

//----------------------------------------------------------------------------
// release all OpenGL ressources
//
void Texture2d::clear()
{
    releaseOpenGlResources();

    mIsValid = false;
    mTarget = tt2d;
    mInternalFormat = tifSrgb8;
    mFormat = tfRgba;
    mDataType = dtUnsignedByte;
    mMagnificationFilter = tfNearest;
    mMinificationFilter = tfNearest;
    mWrapS = twmClampToBorder;
    mWrapT = twmClampToBorder;
    mSizeInPixels.set(0, 0);
    mSizeInBytes = 0;
    mBoundTextureUnit = -1;

    mNumberOfMipmap = 0;
    mNumberOfSamples = 1;
    mMaxAnisotropicFilteringValue = 1.0f;
}

//------------------------------------------------------------------------------
void Texture2d::generateMipmap()
{
    if (getNumberOfMipmaps() > 1)
    {
        glGenerateTextureMipmap(getId()); // opengl 4.5 only

        //valid when all mipmaps are generated.
        mIsValid = true;
    }    
}

//--------------------------------------------------------------------------
DataType Texture2d::getDataType() const
{ return mDataType; }

//--------------------------------------------------------------------------
TextureFormat Texture2d::getFormat() const
{ return mFormat; }

//--------------------------------------------------------------------------
int Texture2d::getHeight() const
{
    return mSizeInPixels.y();
}

//--------------------------------------------------------------------------
int Texture2d::getId() const
{ return mId; }

//--------------------------------------------------------------------------
TextureInternalFormat Texture2d::getInternalFormat() const
{ return mInternalFormat; }

//--------------------------------------------------------------------------
TextureFilter Texture2d::getMagnificationFilter() const
{
    return mMagnificationFilter;
}

//--------------------------------------------------------------------------
double Texture2d::getMaxAnisotropicFilteringValue() const
{
    return mMaxAnisotropicFilteringValue;
}

//--------------------------------------------------------------------------
TextureFilter Texture2d::getMinificationFilter() const
{
    return mMinificationFilter;
}

//--------------------------------------------------------------------------
int Texture2d::getNumberOfChannels() const
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

//------------------------------------------------------------------------------
int Texture2d::getNumberOfMipmaps() const
{
    return mNumberOfMipmap;
}

//------------------------------------------------------------------------------
int Texture2d::getNumberOfSamples() const
{
    return mNumberOfSamples;
}

//--------------------------------------------------------------------------
int Texture2d::getSizeInBytes() const
{
    return mSizeInBytes;
}

//--------------------------------------------------------------------------
Math::Vector2i Texture2d::getSizeInPixels() const
{
    return mSizeInPixels;
}

//--------------------------------------------------------------------------
TextureTarget Texture2d::getTarget() const
{
    return mTarget;
}

//--------------------------------------------------------------------------
int Texture2d::getWidth() const
{
    return mSizeInPixels.x(); 
}

//--------------------------------------------------------------------------
TextureWrapMode Texture2d::getWrapSMode() const
{
    return mWrapS;
}

//--------------------------------------------------------------------------
TextureWrapMode Texture2d::getWrapTMode() const
{
    return mWrapT;
}

//--------------------------------------------------------------------------
bool Texture2d::hasMipmaps() const
{
    return (mNumberOfMipmap>1);
}

//--------------------------------------------------------------------------
bool Texture2d::isCompressed() const
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
bool Texture2d::isValid() const
{
    return mIsValid;
}

//--------------------------------------------------------------------------
void Texture2d::releaseOpenGlResources()
{
    if (getId() != 0)
    {
        glDeleteTextures(1, &mId);
        mId = 0;
    }
}

//--------------------------------------------------------------------------
// Resizes the texture but flush the content. This method is mostly used
// to resize textures that are used with an FBO (as color attachment).
//
// Note: This methods works only when the internal format is uncompressed
//
void Texture2d::resizeFbo(int iWidth, int iHeight)
{
    if(!isCompressed())
    {
		mSizeInPixels.set(iWidth, iHeight);
		mSizeInBytes = iWidth * iHeight * getNumberOfChannels() * getSizeOf(getDataType());

		const GLenum target = toGlTextureTarget(getTarget());
		const GLenum internalFormat = toGlTextureInternalFormat(getInternalFormat());
		const GLenum format = toGlTextureFormat(getFormat());
		const GLenum dataType = toGlDataType(getDataType());

		bind(0);

		if (getNumberOfSamples() > 1)
		{
			glTexImage2DMultisample(target, mNumberOfSamples, internalFormat, getWidth(), getHeight(), GL_TRUE);
		}
		else
		{
			const bool depthTexture = (mFormat == TextureFormat::tfDepth) ||
				(mFormat == TextureFormat::tfDepthStencil) ||
				(mFormat == TextureFormat::tfStencil);
		
			glTexImage2D(target, 0, internalFormat, getWidth(), getHeight(), 0, format, dataType, NULL);
		}
		unbind();
    }
}

//--------------------------------------------------------------------------
void Texture2d::resizeFbo(const Math::Vector2i& iSize)
{
    resizeFbo(iSize.x(), iSize.y());
}

//--------------------------------------------------------------------------
void Texture2d::set(TextureTarget iTarget,
    int iMipMapIndex,
    int iNumberOfMipMaps,
    TextureInternalFormat iInternalFormat,
    int iWidth, 
    int iHeight, 
    TextureFormat iFormat, 
    DataType iDataType, 
    void *ipData)
{
    if ( (mId == 0) || (iTarget != getTarget()) ) // we recreate the texture if the target changed or no previous texture
    {
        mTarget = iTarget;
		const GLenum target = toGlTextureTarget(getTarget());
        
        // release previous if exists
        releaseOpenGlResources();

		glCreateTextures(target,1, &mId);
    }

    updateParameters();

    if (iMipMapIndex == 0)// we reset the texture only if it is the first mipmap
    {
        mNumberOfMipmap = iNumberOfMipMaps;
        mNumberOfSamples = 1;

        mInternalFormat = iInternalFormat;
        mFormat = iFormat;
        mDataType = iDataType;
        mSizeInPixels.set(iWidth, iHeight);
        mSizeInBytes = iWidth * iHeight * getNumberOfChannels() * getSizeOf(getDataType());

        const GLenum internalFormat = toGlTextureInternalFormat(getInternalFormat());
        
    	glTextureStorage2D(getId(), mNumberOfMipmap, internalFormat, getWidth(), getHeight());
    }

    if(ipData)
    {
        const GLenum format = toGlTextureFormat(getFormat());
        const GLenum dataType = toGlDataType(getDataType());

	    // now send the image
        glTextureSubImage2D(getId(),
            iMipMapIndex, // mipmap level
            0,0, // x,y offset
            getWidth(),getHeight(),
            format,dataType,ipData);
    }

    // a texture is valid once all mipmaps have been specified...
    //
    mIsValid = iMipMapIndex == (iNumberOfMipMaps - 1);
}

//--------------------------------------------------------------------------
void Texture2d::setAsCompressed(TextureTarget iTarget,
    int iMipmapIndex, int nbMipmap,
    TextureInternalFormat iInternalFormat,
    int iWidth,
    int iHeight,
    int iImageSizeInBytes,
    void *ipData)
{
    if (mId == 0)
    {
        mNumberOfSamples = 1;
		mNumberOfMipmap = nbMipmap;
        mTarget = iTarget;
        mInternalFormat = iInternalFormat;
        mFormat = tfRgba;
        mDataType = dtUnsignedByte;
        mSizeInPixels.set(iWidth, iHeight);
        mSizeInBytes = iImageSizeInBytes;

		const GLenum target = toGlTextureTarget(getTarget());		
		glCreateTextures(target, 1, &mId);
    }

    GLenum internalFormat = toGlTextureInternalFormat(getInternalFormat());

	updateParameters();

    if(iMipmapIndex == 0)
    {
        glTextureStorage2D(getId(),nbMipmap,internalFormat,getWidth(),getHeight());
    }
    if(ipData)
    {
        glCompressedTextureSubImage2D(getId(),iMipmapIndex,0,0,iWidth,iHeight,internalFormat,
            iImageSizeInBytes,ipData);
    }
    
    // a texture is valid once all mipmaps have been specified...
    //
    mIsValid = iMipmapIndex == (nbMipmap - 1);
}

//--------------------------------------------------------------------------
// This is a very specialized function to cooperate with FrameBufferObject.
// The subtlety here is that the texture can be multisampled, which is
// quite different from the usual texture usage...
//
// see also resizeFbo
//
void Texture2d::setFboTexture(TextureTarget iTarget,
    TextureInternalFormat iInternalFormat,
    int iWidth,
    int iHeight,
    TextureFormat iFormat,
    DataType iDataType,
    int iNumberOfSamples)
{
    if (getId() != 0)
    {
        releaseOpenGlResources();
    }

    mNumberOfMipmap = 1;
    mNumberOfSamples = iNumberOfSamples;
    mTarget = iTarget;
    mInternalFormat = iInternalFormat;
    mFormat = iFormat;
    mDataType = iDataType;

	const GLenum target = toGlTextureTarget(getTarget());
	glCreateTextures(target, 1, &mId);

    resizeFbo(iWidth, iHeight);

    mIsValid = true;
}

//------------------------------------------------------------------------------
void Texture2d::setMagnificationFilter(TextureFilter iTf)
{
    mShouldUpdateParameters = true;
    mMagnificationFilter = iTf;
}

//------------------------------------------------------------------------------
void Texture2d::setMaxAnisotropicFilteringValue(double iV)
{
    mShouldUpdateParameters = true;
    mMaxAnisotropicFilteringValue = (float)iV;
}

//------------------------------------------------------------------------------
void Texture2d::setMinificationFilter(TextureFilter iTf)
{
    mShouldUpdateParameters = true;
    mMinificationFilter = iTf;
}

//------------------------------------------------------------------------------
void Texture2d::setWrapSMode(TextureWrapMode iTwm)
{
    mShouldUpdateParameters = true;
    mWrapS = iTwm;
}

//------------------------------------------------------------------------------
void Texture2d::setWrapTMode(TextureWrapMode iTwm)
{
    mShouldUpdateParameters = true;
    mWrapT = iTwm;
}

//------------------------------------------------------------------------------
void Texture2d::setWrapMode(TextureWrapMode iTwm)
{
    mShouldUpdateParameters = true;
    mWrapS = iTwm;
    mWrapT = iTwm;
}

//------------------------------------------------------------------------------
void Texture2d::unbind() const
{
    if (mBoundTextureUnit != -1)
    {
        glActiveTexture(GL_TEXTURE0 + mBoundTextureUnit);

        const GLenum target = toGlTextureTarget(getTarget());
        glBindTexture(target, 0);
        mBoundTextureUnit = -1;
    }
}

void   Texture2d::copyTo(Texture2d& output, const int& mipmapIndexSrc, const int& mipmapIndexDst) const
{
    if( !output.isValid() || (output.getWidth() != this->getWidth()) || (output.getHeight() != this->getHeight()) )
    {
        output.clear();
        output.setWrapSMode( this->getWrapSMode() );
        output.setWrapTMode( this->getWrapTMode() );
        output.setMinificationFilter(this->getMinificationFilter());
        output.setMagnificationFilter(this->getMagnificationFilter());

        output.set( this->getTarget(), 0, this->getNumberOfMipmaps(), this->getInternalFormat(), this->getWidth(),this->getHeight(),this->getFormat(),this->getDataType(), nullptr);
    }

    if(output.isValid() && glCopyImageSubData && this->isValid() )
    {
        const GLenum srcTarget = toGlTextureTarget(this->getTarget());
        const GLenum dstTarget = toGlTextureTarget(output.getTarget());

        // copy opengl
        glCopyImageSubData( this->getId(),srcTarget,mipmapIndexSrc,0,0,0,
                            output.getId(), dstTarget,mipmapIndexDst,0,0,0,
                            this->getWidth(),this->getHeight(),1);
    }
    else
    {
    }
}

//-----------------------------------------------------------------------------
// This method assumes that the texture is binded. It works with a dirty flag
// see mShouldUpdateParameters
//
void Texture2d::updateParameters() const
{
    if(mShouldUpdateParameters && getId())
    {
        mShouldUpdateParameters = false;
        const GLenum magFilter = toGlTextureFilter(getMagnificationFilter());
        const GLenum minFilter = toGlTextureFilter(getMinificationFilter());
        const GLenum wrapS = toGlTextureWrapMode(getWrapSMode());
        const GLenum wrapT = toGlTextureWrapMode(getWrapTMode());

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // keep it !!

        if(getTarget() != TextureTarget::tt2dMultisampled)
        {
            glTextureParameteri(getId(),GL_TEXTURE_MAG_FILTER,magFilter);
            glTextureParameteri(getId(),GL_TEXTURE_MIN_FILTER,minFilter);

            // if the target it multisample, the sampling parameter can not be changed
            glTextureParameteri(getId(), GL_TEXTURE_WRAP_S, wrapS);
            glTextureParameteri(getId(), GL_TEXTURE_WRAP_T, wrapT);

            glTextureParameterf(getId(), GL_TEXTURE_MAX_ANISOTROPY_EXT, mMaxAnisotropicFilteringValue);
        }

    }
}