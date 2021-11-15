
#include "MainWindow.h"
#include <QApplication>
#include <QSurfaceFormat>

#include "Reactor/Engine.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
//    format.setVersion(3, 2);
//    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);
    
    int exitCode = 0;
    Realisim::Reactor::Engine e;
    if(e.init())
    { 
        //e.start();

        MainWindow m;
        m.show();

        exitCode = app.exec();

        //e.stop();
        e.terminate();
    }
    

    return exitCode;
}
