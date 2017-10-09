#pragma once

#include "DataStructure/ImageCells.h"
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
        ImageCells& getImageCells();
        Scene& getScene();
        FrameBuffer& getFrameBuffer();
        
    protected:
        FrameBuffer mFrameBuffer;
        ImageCells mImageCells;
        Rendering::Camera mCamera;
        Scene mScene;
    };
}
}