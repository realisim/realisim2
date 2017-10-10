#pragma once

#include "DataStructure/ImageCells.h"
#include "DataStructure/Scene.h"
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
        RenderStack& getRenderStack();
        
    protected:
        Core::Image mFinalImage;
        RenderStack mRenderStack;
        Rendering::Camera mCamera;
        Scene mScene;
    };
}
}