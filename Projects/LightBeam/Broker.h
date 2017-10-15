#pragma once

#include "DataStructure/ImageCells.h"
#include "DataStructure/Scene.h"
#include "Raytracer.h"
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
        Core::Image& getFinalImage();
        Scene& getScene();
        ImageCells& getImageCells();
        
    protected:
        Core::Image mFinalImage;
        ImageCells mImageCells;
        Rendering::Camera mCamera;
        Scene mScene;
    };
}
}