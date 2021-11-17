
#pragma once

#include "Interface/Keyboard.h"
#include "Interface/Mouse.h"
#include "Rendering/Camera.h"

namespace Realisim
{
namespace Reactor{
    class Broker
    {
    public:
        Broker() = default;
        Broker(const Broker&) = delete;
        Broker&operator=(const Broker&) = delete;
        ~Broker() = default;
        
        Interface::Keyboard& getKeyboard() { return mKeyboard; }
        const Rendering::Camera& getMainCamera() const { return mMainCamera; }
        Rendering::Camera& getMainCameraRef() { return mMainCamera; }
        Interface::Mouse& getMouse() { return mMouse; }

    protected:
        // scene
        
        Rendering::Camera mMainCamera;
        Interface::Mouse mMouse;
        Interface::Keyboard mKeyboard;

        // stats
    };
}
}