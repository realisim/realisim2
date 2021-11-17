

#include "Interface/Keyboard.h"
#include "Interface/Mouse.h"
#include "Math/CommonMath.h"
#include "Systems/CameraController.h"

using namespace Realisim;
    using namespace Interface;
    using namespace Reactor;

//---------------------------------------------------------------------------------------------------------------------
CameraController::CameraController(Broker* ipBroker, Hub* ipHub) : ISystem(ipBroker, ipHub) {
}

//---------------------------------------------------------------------------------------------------------------------
CameraController::~CameraController(){
}

//---------------------------------------------------------------------------------------------------------------------
void CameraController::handleKeyboard()
{
    Broker& b = getBrokerRef();
    Keyboard& k = b.getKeyboard();
    Rendering::Camera& cam = b.getMainCameraRef();

    const double kDeltaTime = 1 / 60.0f;
    const double kSpeedInMPerSec = 2.0;
    Math::Vector3 displacement(0.0, 0.0, 0.0);

    if (k.isKeyPressed(Realisim::Key_W))
        displacement += cam.getDirection();
    if (k.isKeyPressed(Realisim::Key_S))
        displacement -= cam.getDirection();
    if (k.isKeyPressed(Realisim::Key_A))
        displacement -= cam.getLateralVector();
    if (k.isKeyPressed(Realisim::Key_D))
        displacement += cam.getLateralVector();
    if (k.isKeyPressed(Realisim::Key_Q))
        displacement += cam.getUp();
    if (k.isKeyPressed(Realisim::Key_E))
        displacement -= cam.getUp();

    displacement *= kSpeedInMPerSec * kDeltaTime;
    cam.translate(displacement);
}

//---------------------------------------------------------------------------------------------------------------------
void CameraController::handleMouse()
{
    Broker& b = getBrokerRef();
    Mouse& m = b.getMouse();
    Rendering::Camera& cam = b.getMainCameraRef();

    if (m.isButtonPressed(Mouse::bLeft)) {

        // 180 degree for window width
        const float degreesPerPixel = 90.0f / cam.getViewport().getWidth();
        const double radiansPerPixel = degreesToRadians(degreesPerPixel);

        Math::Vector2 d(m.getAndClearDelta());
        d *= radiansPerPixel;

        cam.rotate(-d.x(), cam.getUp(), cam.getPosition());
        cam.rotate(-d.y(), cam.getLateralVector(), cam.getPosition());
        cam.set(cam.getPosition(), cam.getFocal(), Math::Vector3(0, 1, 0));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void CameraController::handleGamepad()
{}

//---------------------------------------------------------------------------------------------------------------------
void CameraController::update() {
    handleKeyboard();
    handleMouse();
    handleGamepad();
}