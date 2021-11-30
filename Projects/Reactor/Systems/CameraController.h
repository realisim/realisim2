#pragma once

#include "Systems/ISystem.h"

namespace Realisim
{
namespace Reactor
{
    class CameraController : public ISystem
    {
    public:
        CameraController() = delete;
        CameraController(Broker* ipBroker, Hub* ipHub);
        CameraController(const CameraController&) = delete;
        CameraController& operator=(const CameraController&) = delete;
        ~CameraController();

        virtual void update() override final;

    protected:
        void handleKeyboard();
        void handleMouse();
        void handleGamepad();

        double mCameraSpeedInMPerSec;
    };
}
}

