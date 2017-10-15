#pragma once

#include "DataStructure/ImageCells.h"
#include "DataStructure/Scene.h"
#include "Interface/Keyboard.h"
#include "Interface/Mouse.h"
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
        Interface::Keyboard& getKeyboard();
        Interface::Mouse& getMouse();
        Scene& getScene();
        ImageCells& getImageCells();
        
    protected:
        Interface::Keyboard mKeyboard;
        Interface::Mouse mMouse;
    
        Core::Image mFinalImage;
        ImageCells mImageCells;
        Rendering::Camera mCamera;
        Scene mScene;
    };
}
}