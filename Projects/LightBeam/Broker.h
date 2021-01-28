#pragma once

#include "DataStructure/Scene/Scene.h"
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

        Core::Image& getFinalImage();
        Interface::Keyboard& getKeyboard();
        Interface::Mouse& getMouse();
        const Scene& getScene() const;
        Scene& getSceneRef();
        
    protected:
        Interface::Keyboard mKeyboard;
        Interface::Mouse mMouse;
    
        Core::Image mFinalImage;
        Scene mScene;
    };
}
}