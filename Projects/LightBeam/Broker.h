#pragma once

#include "DataStructure/Scene.h"
#include "FrameBuffer.h"
#include "Rendering/Camera.h"

namespace Realisim
{
namespace LightBeam
{
    class Broker
    {
    public:
        Broker();
        ~Broker();

        Rendering::Camera& getCamera();
        Scene& getScene();
        FrameBuffer& getFrameBuffer();
        
    protected:
        FrameBuffer mFrameBuffer;
        Rendering::Camera mCamera;
        Scene mScene;
    };
}
}