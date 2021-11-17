
#pragma once

#include <array>
#include <map>

namespace Realisim
{
namespace Reactor{

    class CameraController;
    class ISystem;
    class Renderer;

    class Hub
    {
    public:
        friend class Engine;

        Hub() = default;
        Hub(const Hub&) = delete;
        Hub&operator=(const Hub&) = delete;
        ~Hub() = default;
        
        enum System {sRenderer = 0, sNumberOfSystems, sUserDefined = 10000};

        CameraController& getCameraController() { return *mpCameraController; }

        const Renderer& getRenderer() const { return *mpRenderer; }
        Renderer& getRendererRef() const { return *mpRenderer; };

        ISystem* getSystem(System);
        ISystem* getUserDefinedSystem(int iSystem);

    protected:
        void clear(); // only called by Engine.

        // not owned pointers to all core systems
        Renderer *mpRenderer;
        CameraController *mpCameraController;

        // owned pointer to all userDefined systems
        std::map<int, ISystem*> mUserDefinedSystems;
    };
}
}