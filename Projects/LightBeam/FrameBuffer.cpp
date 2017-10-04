
#include "FrameBuffer.h"

using namespace Realisim;
    using namespace Core;
    using namespace LightBeam;
    using namespace Math;
//-----------------------------------------------------------------------------
FrameBuffer::FrameBuffer() :
    mIsValid(false),
    mSize(0, 0),
    mColorBuffer(),
    mDepthBuffer()
{}

//-----------------------------------------------------------------------------
FrameBuffer::~FrameBuffer()
{}

//-----------------------------------------------------------------------------
void FrameBuffer::clear()
{
    mIsValid = false;
    mSize = Vector2i();
    mColorBuffer.clear();
    mDepthBuffer.clear();
}

//-----------------------------------------------------------------------------
Core::Image& FrameBuffer::getColorBuffer()
{ return mColorBuffer; }

//-----------------------------------------------------------------------------
Core::Image& FrameBuffer::getDepthBuffer()
{ return mDepthBuffer; }

//-----------------------------------------------------------------------------
Math::Vector2i FrameBuffer::getSize() const
{ return mSize; }

//-----------------------------------------------------------------------------
bool FrameBuffer::isValid() const
{ return mIsValid; }

//-----------------------------------------------------------------------------
void FrameBuffer::setSize(const Math::Vector2i& iSize)
{
    if(getSize() != iSize)
    {
        clear();

        mSize = iSize;
        mColorBuffer.set(iSize, iifRgbaUint8);
        mDepthBuffer.set(iSize, iifRF32);
        mIsValid = true;
    }
}