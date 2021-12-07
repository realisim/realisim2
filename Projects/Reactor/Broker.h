
#pragma once

#include "Core/Timer.h"
#include "DataStructures/Scene/Scene.h"
#include "Interface/Keyboard.h"
#include "Interface/Mouse.h"
#include "Rendering/Camera.h"

namespace Realisim
{
namespace Reactor{
    class Broker
    {
    public:
        Broker();
        Broker(const Broker&) = delete;
        Broker&operator=(const Broker&) = delete;
        ~Broker() = default;
        
        const std::string& getAssetPath() const { return mAssetPath; }
        const Interface::Keyboard& getKeyboard() const { return mKeyboard; }
        Interface::Keyboard& getKeyboardRef() { return mKeyboard; }
        const Rendering::Camera& getMainCamera() const { return mMainCamera; }
        Rendering::Camera& getMainCameraRef() { return mMainCamera; }
        Interface::Mouse& getMouse() { return mMouse; }
        double getTimeInSecondsOfStartOfFrame() const { return mTimeOfStartOfFrame ; }
        double getTimeInSecondsSinceStartup() const { return mTimeSinceStartup.elapsed(); }
        const Scene& getScene() const { return mScene; }
        Scene& getSceneRef() { return mScene; }

        void makeBasicScene();

        void setStartOfFrameTime() { mTimeOfStartOfFrame = mTimeSinceStartup.elapsed(); }

    protected:
        // scene
        Scene mScene;
        Rendering::Camera mMainCamera;
        Interface::Mouse mMouse;
        Interface::Keyboard mKeyboard;
        std::string mAssetPath;

        // stats
        Core::Timer mTimeSinceStartup;
        double mTimeOfStartOfFrame;
    };
}
}