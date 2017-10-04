
#include "Broker.h"
#include "MainWindow.h"
#include <QApplication>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    
    Realisim::LightBeam::Broker b;
    Realisim::LightBeam::MainWindow m(&b);
    
    m.show();
    
    return app.exec();
}
