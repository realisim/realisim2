#pragma once

#include "Core/Image.h"
#include <memory>

namespace Realisim
{
namespace LightBeam
{
    class FrameBuffer
    {
    public:
        FrameBuffer();
        FrameBuffer(const FrameBuffer&) = default;
        FrameBuffer& operator=(const FrameBuffer&) = default;
        ~FrameBuffer();

        void clear();
        Core::Image& getColorBuffer();
        Core::Image& getDepthBuffer();
        Math::Vector2i getSize() const;
        bool isValid() const;
        void setSize(const Math::Vector2i& iSize);

    protected:
        bool mIsValid;
        Math::Vector2i mSize;

        Core::Image mColorBuffer;
        Core::Image mDepthBuffer;
    };
}
}