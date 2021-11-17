
#pragma once

#include "Broker.h"
#include "Hub.h"
#include "Systems/ISystem.h"
#include <vector>

#ifdef _WIN32
    // for HDC and HGLRC
    #include <Windows.h>
#endif // WIN32

namespace Realisim
{
namespace Reactor{
    class Engine
    {
    public:
        Engine() = default;
        Engine(const Engine&) = delete;
        Engine& operator=(const Engine&) = delete;
        ~Engine() = default;

        bool init();
        const Broker& getBroker() const { return mBroker; }
        Broker& getBrokerRef() { return mBroker; }
        const Hub& getHub() const { return mHub; }
        Hub& getHubRef() { return mHub; }

        void update();
        //void start();
        //void stop();
        void setNativeWindowsGlContext(HDC iHDC, HGLRC iHGLRC);
        void terminate();

    protected:
        void createCoreSystems();

        Broker mBroker;
        Hub mHub;
        std::vector<ISystem*> mSystems; // owned pointers to systems
    };
}
}