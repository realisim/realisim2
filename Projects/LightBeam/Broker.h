#pragma once

#include "FrameBuffer.h"

namespace Realisim
{
namespace LightBeam
{
    class Broker
    {
    public:
        Broker();
        ~Broker();

        FrameBuffer& getFrameBuffer();
        
    protected:
        FrameBuffer mFrameBuffer;
    };
}
}