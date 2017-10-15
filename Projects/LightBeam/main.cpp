
#include "Broker.h"
#include "MainWindow.h"
#include <QApplication>
#include "Raytracer.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    
    Realisim::LightBeam::Broker b;
    Realisim::LightBeam::RayTracer rayTracer(&b);
    Realisim::LightBeam::MainWindow m(&b, &rayTracer);
    
    m.show();
    
    return app.exec();
}
