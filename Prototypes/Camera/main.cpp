
#include "MainWindow.h"
#include <QApplication>
#include <QSurfaceFormat>


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
//    format.setVersion(3, 2);
//    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);
    
    MainWindow m;
    m.show();
    
    return app.exec();
}
