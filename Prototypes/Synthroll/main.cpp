
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
#include "Systems/GameSystem.h"
#include "Systems/PhysicsSystem.h"
#include "Systems/SystemIds.h"
#include "Systems/TerrainEditionSystem.h"

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
    Reactor::Broker& b = engine.getBrokerRef();
    Reactor::Hub& h = engine.getHubRef();
    Reactor::CameraController* pCameraController = new Reactor::CameraController(&b, &h);
    engine.addUserSystem(Reactor::Hub::usiCameraController, pCameraController);

    // create and add all systems
    //GameSystem
    Synthroll::GameSystem* gs = new Synthroll::GameSystem(&b, &h);
    engine.addUserSystem(GAME_SYSTEM_ID, gs);

    //PhysicsSystem*
    Synthroll::PhysicsSystem* ps = new Synthroll::PhysicsSystem(&b, &h);
    engine.addUserSystem(PHYSICS_SYSTEM_ID, ps);

    //TerrainEdition
    Synthroll::TerrainEditionSystem* tes = new Synthroll::TerrainEditionSystem(&b, &h);
    engine.addUserSystem(TERRAIN_EDITION_SYSTEM_ID, tes);

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
