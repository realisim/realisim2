
#include "Core/Logger.h"
#include "MainWindow.h"

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QSurfaceFormat>
#include <QScreen>
#include "Reactor/Engine.h"
#include "Reactor/Systems/CameraController.h"

using namespace Realisim;
using namespace Reactor;
using namespace std;

//-----------------------------------------------------------------------------
void configureGlSurface()
{
    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setSwapInterval(1);
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    format.setSamples(4);
    //format.setSwapBehavior(QSurfaceFormat::TripleBuffer);
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    //format.setVersion(4, 5);
    //format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);
}

//-----------------------------------------------------------------------------
int main(int argc, char** argv)
{    
    LOG_TRACE(Core::Logger::llNormal, "Start of Reactor-EngineProto");

    QApplication app(argc, argv);

    // configure GLSurface
    configureGlSurface();
    
    //
    Reactor::Engine engine;

    // temporary camera controller
    Realisim::Reactor::Hub& h = engine.getHubRef();
    Realisim::Reactor::CameraController* pCameraController = new Realisim::Reactor::CameraController(&engine.getBrokerRef(), &h);
    engine.addUserSystem(Realisim::Reactor::Hub::usiCameraController, pCameraController);

    caca

    // create and add all systems
    //NetworkSystem
    //GameSystem*
    //PhysicsSystem*
    //PlayerControllerSystem
    

    //QScreen* pScreen = app.primaryScreen();
    //engine.setScreenRefreshrateInMsecs(1.0 / (double)pScreen->refreshRate());
       
    int exitCode = -1;
    if (engine.init())
    {
        MainWindow mw(&engine);
        mw.show();
        exitCode = app.exec();
    }
    
    return exitCode;
}
